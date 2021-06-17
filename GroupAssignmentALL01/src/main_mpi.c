#include <mpi.h>
#include	<stdlib.h>
#include <stdio.h>
#include "knn.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main(int argc, char *argv[])
{	
	int rank,size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	double time_generate = 0.0, time_classify = 0.0;

	if(argc != 5){
		fprintf(stderr,"Usage:\n\t%s [n : points to generate] [x: Coordinate x of the point to classify] [y: Coordinate y of the point to classify] [k]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
	int k = atoi(argv[4]);

	Point dataset[n];
	Point test_point;

	test_point.x = x;
	test_point.y = y;
	
	if(rank == 0){
		double start_time_generate = MPI_Wtime();
		generate_points(dataset, n, 1);
		double end_time_generate = MPI_Wtime();
		time_generate = end_time_generate - start_time_generate;
#ifdef DEBUG
		printf("Points generated\n");
#endif
	}

	MPI_Datatype point_type;
	MPI_Type_create_Point(&point_type);
	MPI_Type_commit(&point_type);
	MPI_Bcast(dataset,n,point_type,0,MPI_COMM_WORLD);
	MPI_Type_free(&point_type);

	double start_time_classify = MPI_Wtime();
	test_point.cluster_number = MPI_classify_point(dataset,test_point,k,n);
	double end_time_classify = MPI_Wtime();
	time_classify = end_time_classify - start_time_classify;

	double global_time_generate,global_time_classify;
	MPI_Reduce(&time_generate,&global_time_generate,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Reduce(&time_classify,&global_time_classify,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	
	if(rank == 0)printf("%d;%d;%f;%f\n", n, size, time_generate, time_classify);
#ifdef DEBUG
	if(rank == 0)printf("Point belongs to cluster: %d\n", test_point.cluster_number);
#endif

	exit(EXIT_SUCCESS);
}				/* ----------  end of function main  ---------- */
