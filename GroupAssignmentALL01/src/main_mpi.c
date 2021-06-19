#include <mpi.h>
#include    <stdlib.h>
#include <stdio.h>
#include "knn.h"

/*
 * ===  FUNCTION  ======================================================================
 *      Name:  main
 *      Description:
 * =====================================================================================
 */
int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 4) {
        if (rank == 0) {
            fprintf(stderr, "Usage:\n\t%s [n : points to generate] [k] [#clusters]\n",
                    argv[0]);
        }
        exit(EXIT_FAILURE);
    }

    double time_generate = 0.0, time_classify = 0.0;

    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int num_clusters = atoi(argv[3]);


    if (n < size || k < size || num_clusters < size) {
        if (rank == 0)fprintf(stderr,
                                  "Exiting ... n, k, num_clusters must be greater than P\n");
        exit(EXIT_FAILURE);
    }

    if (((n % size) != 0 || (k % size) != 0 || (num_clusters % size) != 0)) {
        if (rank == 0) {
            fprintf(stderr, "Exiting ... n, k, num_clusters must be a multiple of P\n");
        }
        exit(EXIT_FAILURE);
    }

    Point *dataset = (Point *) malloc(n * sizeof(Point));
    Point test_point;

    test_point.x = ((float)RAND_MAX) / (k * 7.6);
    test_point.y = ((float)RAND_MAX) / (k * 7.6);

    if (rank == 0) {
        double start_time_generate = MPI_Wtime();
        //no -fopenmp so num_threads can be aribitrary
        generate_points(dataset, n, 1, num_clusters);
        double end_time_generate = MPI_Wtime();
        time_generate = end_time_generate - start_time_generate;
#ifdef DEBUG
        printf("Points generated\n");
        printf("%d\n", __LINE__);
#endif
    }

    MPI_Datatype point_type;
    MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);
    MPI_Bcast(dataset, n, point_type, 0, MPI_COMM_WORLD);

    double start_time_classify = MPI_Wtime();
    test_point.cluster_number = MPI_classify_point(dataset, test_point, k, n,
                                num_clusters);
    double end_time_classify = MPI_Wtime();
    time_classify = end_time_classify - start_time_classify;

    double global_time_generate, global_time_classify;
    MPI_Reduce(&time_generate, &global_time_generate, 1, MPI_DOUBLE, MPI_MAX, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&time_classify, &global_time_classify, 1, MPI_DOUBLE, MPI_MAX, 0,
               MPI_COMM_WORLD);

    //MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)printf("%d;%d;%d;%d;%f;%f;%f\n", n, k, num_clusters, size,
                             time_generate, time_classify, time_classify);

#ifdef DEBUG
    if (rank == 0)printf("Point belongs to cluster: %d\n",
                             test_point.cluster_number);
#endif

    MPI_Type_free(&point_type);
    exit(EXIT_SUCCESS);
}               /* ----------  end of function main  ---------- */
