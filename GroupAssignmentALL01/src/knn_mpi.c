#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "knn.h"

// https://stackoverflow.com/questions/23633916/how-does-mpi-odd-even-sort-work

int mpi_merge(Point *ina, int lena, Point *inb, int lenb, Point *out) {
    int i,j;
    int outcount=0;

    for (i=0,j=0; i<lena; i++) {
        while ((inb[j].distance < ina[i].distance) && j < lenb) {
            out[outcount++] = inb[j++];
        }
        out[outcount++] = ina[i];
    }
    while (j<lenb)
        out[outcount++] = inb[j++];

    return 0;
}

void printstat(int rank, int iter, char *txt, Point *la, int n) {
    printf("[%d] %s iter %d: >\n", rank, txt, iter);
    for (int j=0; j<n-1; j++)
		printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n", \
				la[j].cluster_number, la[j].x, la[j].y, la[j].distance);
	 
	printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n", \
			la[n-1].cluster_number, la[n-1].x, la[n-1].y, la[n-1].distance);
}

void MPI_Pairwise_Exchange(int localn, Point *locala, int sendrank, int recvrank,MPI_Comm comm) {

    /*
     * the sending rank just sends the data and waits for the results;
     * the receiving rank receives it, sorts the combined data, and returns
     * the correct half of the data.
     */
    int rank;
    Point remote[localn];
    Point all[2*localn];
    const int mergetag = 1;
    const int sortedtag = 2;
	 // Create the datatype
    MPI_Datatype point_type;
	 MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);

    MPI_Comm_rank(comm, &rank);
    if (rank == sendrank) {
        MPI_Send(locala, localn, point_type, recvrank, mergetag, MPI_COMM_WORLD);
        MPI_Recv(locala, localn, point_type, recvrank, sortedtag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(remote, localn, point_type, sendrank, mergetag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        mpi_merge(locala, localn, remote, localn, all);

        int theirstart = 0, mystart = localn;
        if (sendrank > rank) {
            theirstart = localn;
            mystart = 0;
        }
        MPI_Send(&(all[theirstart]), localn, point_type, sendrank, sortedtag, MPI_COMM_WORLD);
        for (int i=mystart; i<mystart+localn; i++)
            locala[i-mystart] = all[i];
    }
	 MPI_Type_free(&point_type);
}

int MPI_Type_create_Point(MPI_Datatype *point_type){
	 int lengths[2] = { 1, 3 };

	 MPI_Aint displacements[2];
    Point dummy_point;
    MPI_Aint base_address;
    MPI_Get_address(&dummy_point, &base_address);
    MPI_Get_address(&dummy_point.cluster_number, &displacements[0]);
    MPI_Get_address(&dummy_point.x, &displacements[1]);
    displacements[0] = MPI_Aint_diff(displacements[0], base_address);
    displacements[1] = MPI_Aint_diff(displacements[1], base_address);
    MPI_Datatype types[2] = { MPI_INT, MPI_FLOAT };
    return MPI_Type_create_struct(2, lengths, displacements, types, point_type);
}

int MPI_OddEven_Sort(int n, Point *a, int root, MPI_Comm comm)
{
    int rank, size, i;
    Point *local_a;

// get rank and size of comm
    MPI_Comm_rank(comm, &rank); //&rank = address of rank
    MPI_Comm_size(comm, &size);

    local_a = (Point *) calloc(n / size, sizeof(Point));


	 // Create the datatype
    MPI_Datatype point_type;
	 MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);

// scatter the array a to local_a
    MPI_Scatter(a, n / size, point_type, local_a, n / size, point_type,
        root, comm);
// sort local_a
    QuickSortIterative(local_a, n/size);

//odd-even part
    for (i = 1; i <= size; i++) {
#ifdef DEBUG
        printstat(rank, i, "before", local_a, n/size);
#endif

        if ((i + rank) % 2 == 0) {  // means i and rank have same nature
            if (rank < size - 1) {
                MPI_Pairwise_Exchange(n / size, local_a, rank, rank + 1, comm);
            }
        } else if (rank > 0) {
            MPI_Pairwise_Exchange(n / size, local_a, rank - 1, rank, comm);
        }

    }

#ifdef DEBUG
    printstat(rank, i-1, "after", local_a, n/size);
#endif

// gather local_a to a
    MPI_Gather(local_a, n / size, point_type, a, n / size, point_type,
           root, comm);

	 MPI_Type_free(&point_type);
#ifdef DEBUG
    if (rank == root)
        printstat(rank, i, " all done ", a, n);
#endif

    return MPI_SUCCESS;
}

// ORIGINAL CODE
int MPI_classify_point(Point *dataset, Point test_point, int k, int n){
	int rank,size,i,z;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	
	int local_len;
	if(rank == size-1)
		local_len = n - (size-1)*(n/size);
	else
		local_len = n / size;
#ifdef DEBUG
	printf("[%d] Local len: %d\n",rank,local_len);
#endif

	Point local_dataset[local_len];

   for(i = rank*(n/size), z = 0; z < local_len; z++,i++)
   {
		local_dataset[z] = dataset[i];
   	local_dataset[z].distance = euclidean_distance(dataset[i], test_point);
		//printf("[%d] computed distance %.2f\n",rank,local_dataset[z].distance);
   }

	 // Create the datatype
    MPI_Datatype point_type;
	 MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);
	 MPI_Gather(local_dataset,n/size,point_type,dataset,n/size,point_type,0,MPI_COMM_WORLD);	
	 if(rank == size - 1 && size > 1){
	    int rem_elms = n % size;
	    int starting_local_index = local_len - rem_elms;
	    MPI_Send(local_dataset + starting_local_index,rem_elms,point_type,\
	   		 0,0,MPI_COMM_WORLD);
	 }

	 if(rank == 0 && size > 1){
	    int rem_elms = n % size;
	    MPI_Recv(dataset + n - rem_elms,rem_elms,point_type,size-1,\
	   		 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
	    printstat(0,0,"Checking after gather",dataset,n);
#endif
	 }

	 MPI_OddEven_Sort(n,dataset,0,MPI_COMM_WORLD);

	 // Majority voting
	 int local_k;
	 if(rank == size - 1)
		 local_k = k - (size-1)*(k/size);
	 else
		 local_k = k / size;

	int local_counter_cluster_0 = 0;
   int local_counter_cluster_1 = 0;
   for (i = rank*(k/size), z = 0; z < local_k; z++,i++)
	{
      if (dataset[i].cluster_number == 0)
         local_counter_cluster_0 += 1;
      else
         local_counter_cluster_1 += 1;
   }

#ifdef DEBUG
	printf("[%d] Majority voting 0:%d 1:%d\n",rank,local_counter_cluster_0,local_counter_cluster_1);
#endif

	int global_counter_cluster_0, global_counter_cluster_1;
	MPI_Allreduce(&local_counter_cluster_0,&global_counter_cluster_0,\
			1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
	MPI_Allreduce(&local_counter_cluster_1,&global_counter_cluster_1,\
			1,MPI_INTEGER,MPI_SUM,MPI_COMM_WORLD);
	 
	MPI_Type_free(&point_type);
	return (global_counter_cluster_1 >= global_counter_cluster_0) ? 1 : 0; 
}

