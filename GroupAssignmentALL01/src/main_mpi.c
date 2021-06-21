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
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include "knn_mpi.h"

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Load command line arguments
    if (argc != 4) {
        if (rank == 0) {
            fprintf(stderr, "Usage:\n\t%s [n : points to generate] [k] [#clusters]\n",
                    argv[0]);
        }
        exit(EXIT_FAILURE);
    }
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);
    int num_clusters = atoi(argv[3]);


	// Check if provided values are valid
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

	// Generate a random test point
    Point test_point;
    test_point.x = ((float)RAND_MAX) / (k * 7.6);
    test_point.y = ((float)RAND_MAX) / (k * 7.6);

	// Generate the dataset using the root process
    double time_generate = 0.0;
    if (rank == 0) {
        double start_time_generate = MPI_Wtime();
        
		Point_GeneratePoints(dataset, n, 1, num_clusters);
        
		double end_time_generate = MPI_Wtime();
        time_generate = end_time_generate - start_time_generate;

#ifdef DEBUG
        printf("Points generated\n");
        printf("%d\n", __LINE__);
#endif
    }

	// Now dataset has to be distributed among all the processes
    MPI_Datatype point_type;
    MPI_Type_create_Point(&point_type);
    MPI_Type_commit(&point_type);
    MPI_Bcast(dataset, n, point_type, 0, MPI_COMM_WORLD);

    double start_time_classify = MPI_Wtime();
    test_point.cluster_number = MPI_ClassifyPoint(dataset, test_point, k, n,
                                num_clusters);
    double end_time_classify = MPI_Wtime();
    double time_classify = end_time_classify - start_time_classify;

	// Find the maximum elapsed times among all the processes
    double global_time_generate, global_time_classify;
    MPI_Reduce(&time_generate, &global_time_generate, 1, MPI_DOUBLE, MPI_MAX, 0,
               MPI_COMM_WORLD);
    MPI_Reduce(&time_classify, &global_time_classify, 1, MPI_DOUBLE, MPI_MAX, 0,
               MPI_COMM_WORLD);

    if (rank == 0)printf("%d;%d;%d;%d;%f;%f;%f\n", n, k, num_clusters, size,
                             time_generate, time_classify, time_classify);

#ifdef DEBUG
    if (rank == 0)printf("Point belongs to cluster: %d\n",
                             test_point.cluster_number);
#endif

    MPI_Type_free(&point_type);
    exit(EXIT_SUCCESS);
}               /* ----------  end of function main  ---------- */
