/*
 * Course: High Performance Computing 2020/2021
 *
 * Lecturer: Francesco Moscato  fmoscato@unisa.it
 *
 * Group:
 * Capitani Giuseppe    0622701085  g.capitani@studenti.unisa.it
 * Falanga  Armando 0622701140  a.falanga13@studenti.unisa.it
 * Terrone  Luigi       0622701071  l.terrone2@studenti.unisa.it
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of GroupAssignment01.
 *
 * GroupAssignmentALL01 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GroupAssignment01 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GroupAssignmentALL01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "knn_mpi.h"
#include "point.h"
#include "sort.h"

// https://stackoverflow.com/questions/23633916/how-does-mpi-odd-even-sort-work



void MPI_PairwiseExchange(int localn, Point *locala, int sendrank,
                           int recvrank, MPI_Comm comm)
{
    /*
     * the sending rank just sends the data and waits for the results;
     * the receiving rank receives it, sorts the combined data, and returns
     * the correct half of the data.
     */
#ifdef DEBUG
    printf("Rank %d sending to rank %d on %d elements\n", sendrank, recvrank,
           localn);
#endif
    int rank;
    Point *remote;
    remote = (Point *)malloc(localn * sizeof(Point));
    Point *all;
    all = (Point *)malloc(2 * localn * sizeof(Point));
    const int mergetag = 1;
    const int sortedtag = 2;
#ifdef DEBUG
    printf("[%d] Creating Point datatype\n", rank);
#endif
    // Create the datatype
    MPI_Datatype point_type;
    MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);

    MPI_Comm_rank(comm, &rank);
    if (rank == sendrank) {
#ifdef DEBUG
        printf("[%d] Sending to %d\n", rank, recvrank);
#endif
        MPI_Send(locala, localn, point_type, recvrank, mergetag,
                 MPI_COMM_WORLD);
#ifdef DEBUG
        printf("[%d] Receiving from %d\n", rank, recvrank);
#endif
        MPI_Recv(locala, localn, point_type, recvrank, sortedtag,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
#ifdef DEBUG
        printf("[%d] Receiving from %d\n", rank, sendrank);
#endif
        MPI_Recv(remote, localn, point_type, sendrank, mergetag,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
        printf("[%d] Merging\n", rank);
#endif
        merge(locala, localn, remote, localn, all);

        int theirstart = 0, mystart = localn;
        if (sendrank > rank) { //entra mai qua dentro ?
            theirstart = localn;
            mystart = 0;
        }
#ifdef DEBUG
        printf("[%d] Sending to %d\n", rank, sendrank);
#endif
        MPI_Send(&(all[theirstart]), localn, point_type, sendrank, sortedtag,
                 MPI_COMM_WORLD);
        for (int i = mystart; i < mystart + localn; i++)
            locala[i - mystart] = all[i];
    }
    MPI_Type_free(&point_type);
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
    // when function is called only rank 0 has both space allocated
    // and elements initialized. So we scatter from root (0)
    // to all other processes.
    //              Send               |            Recv              | from on comm
    MPI_Scatter(a, n / size, point_type, local_a, n / size, point_type, root,
                comm);
    // sort local_a
    QuickSortIterative(local_a, n/size);
    //bitonicSequenceGenerator(0, n / size - 1, local_a, 0);

    // odd-even part, what doeas rank 0 do ? Rank 0 only
    // sends to rank 1 and only receives from rank 1 (later),
    // only sends to his successor (1)
    // Look at the gather below
#ifdef DEBUG
    Point_Print( -1, "before pairwise", local_a, 5);
    printf("%d\n", __LINE__);
#endif
    for (i = 1; i <= size; i++) {

        // starting from odd ranked processes to exchange
        if ((i + rank) % 2 == 0) {  // means i and rank have same nature
            if (rank < size - 1) {
                //if i is odd, odd processes send to even processes
                //and wait for something from even
                //processes (each odd process from its successor)
                MPI_PairwiseExchange(n / size, local_a, rank, rank + 1, comm);
            }
        } else if (rank > 0) {
            MPI_PairwiseExchange(n / size, local_a, rank - 1, rank, comm);
        }

#ifdef DEBUG
        printf("%d\n", __LINE__);
        MPI_Barrier(MPI_COMM_WORLD);
#endif

    }

#ifdef DEBUG
    Point_Print( i - 1, "after", local_a, 5);
    printf("%d\n", __LINE__);
#endif

    //gather local_a to a
    MPI_Allgather(local_a, n / size, point_type,
                  a, n / size, point_type, comm);

    MPI_Type_free(&point_type);


    return MPI_SUCCESS;
}

int MPI_ClassifyPoint(Point *dataset, Point test_point, int k, int n,
                       int num_clusters)
{
    int rank, size, i, z;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_len;
    if (rank == size - 1)
        local_len = n - (size - 1) * (n / size);
    else
        local_len = n / size;

#ifdef DEBUG
    printf("[%d] Local len: %d\n", rank, local_len);
    printf("%d\n", __LINE__);
#endif

    Point *local_dataset = (Point *)malloc(local_len * sizeof(Point));

    for (i = rank * (n / size), z = 0; z < local_len; z++, i++) {
        local_dataset[z] = dataset[i];
        local_dataset[z].distance = Point_EuclideanDistance(dataset[i], test_point);
    }

    // Create the datatype
    MPI_Datatype point_type;
    MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);
    MPI_Gather(local_dataset, n / size, point_type, dataset,
               n / size, point_type, 0, MPI_COMM_WORLD);

    //Each process has already given his local chunk with MPI_Gather,
    //only the last one needs to send the last elements remaining.
    //In our case the size is a multiple of the number fo processes,
    //therefore this should not be needed.
    if (rank == size - 1 && size > 1) {
        int rem_elms = n % size;
        int starting_local_index = local_len - rem_elms;
        MPI_Send(local_dataset + starting_local_index, rem_elms, point_type, \
                 0, 0, MPI_COMM_WORLD);
    }

    //receiving remaining elements on rank 0.
    if (rank == 0 && size > 1) {
        int rem_elms = n % size;
        MPI_Recv(dataset + n - rem_elms, rem_elms, point_type, size - 1, \
                 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
#ifdef DEBUG
        Point_Print( 0, "Checking after gather", dataset, n);
        printf("%d\n", __LINE__);
#endif
    }

    MPI_OddEven_Sort(n, dataset, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

#ifdef DEBUG
    printf("%d\n", __LINE__);
    if (rank == 0)
        Point_Print( 0, " all done ", dataset, k);
#endif

    // Majority voting
    int local_k;
    if (rank == size - 1)
        local_k = k - (size - 1) * (k / size);
    else
        local_k = k / size;

    int *local_clusters = calloc(num_clusters, sizeof(int));
    int *global_clusters = calloc(num_clusters, sizeof(int));


    for (i = rank * (k / size), z = 0; z < local_k; z++, i++) {
        local_clusters[dataset[i].cluster_number]++;
    }

#ifdef DEBUG
    for (i = rank * (num_clusters / size), z = 0; z < num_clusters ; z++, i++) {
        printf("[%d] Local Cluster n_%d : %d\n", rank, z, local_clusters[i]);
    }
#endif

    int global_counter_cluster_0, global_counter_cluster_1;
    MPI_Allreduce(local_clusters, global_clusters, num_clusters, MPI_INTEGER,
                  MPI_SUM, MPI_COMM_WORLD);
    //number of clusters is a multiple of the number of processes
    typedef struct {
        int pos;
        int val;
    } max_struct;

    max_struct local_max, global_max;

    local_max.pos = 0;
    local_max.val = global_clusters[0];
    for (i = rank * (num_clusters / size), z = 0; z < num_clusters / size;
            z++, i++) {
        if (local_max.val < global_clusters[i]) {
            local_max.val = global_clusters[i];
            local_max.pos = i;
        }
    }
    //MAXLOC is pointwise so it gives the maximum element between all
    //same positioned elements in all of the given arrays (from processes)
    //-----------------
    //| a | b | c | d | are arrays
    //| x | x | x | x |--> max is searched in same positions across the arrays
    //| . | . | . | . |
    //| . | . | . | . |
    //-----------------
    MPI_Allreduce(&local_max, &global_max, 1, MPI_2INT, MPI_MAXLOC,
                  MPI_COMM_WORLD);
#ifdef DEBUG
    if (rank == 0) {
        printf("val: %d pos: %d\n", global_max.val, global_max.pos);
    }
#endif
    MPI_Type_free(&point_type);
    return global_max.pos;
}

