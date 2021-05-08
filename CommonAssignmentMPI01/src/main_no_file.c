/* 
 * Course: High Performance Computing 2020/2021 
 * 
 * Lecturer: Francesco Moscato	fmoscato@unisa.it 
 *
 * Group: 
 * Capitani	Giuseppe	0622701085	g.capitani@studenti.unisa.it 
 * Falanga	Armando	0622701140  a.falanga13@studenti.unisa.it 
 * Terrone	Luigi		0622701071  l.terrone2@studenti.unisa.it 
 *
 * Copyright (C) 2021 - All Rights Reserved 
 *
 * This file is part of CommonAssignmentMPI01. 
 *
 * CommonAssignmentMPI01 is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 *
 * CommonAssignmentMPI01 is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with CommonAssignmentMPI01.  If not, see <http://www.gnu.org/licenses/>. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "utils.h"

#define COUNT_TAG (1)
#define ROW_TAG (2)
#define B_TAG (3)
#define C_TAG (4)

int main(int argc, char **argv) {

	double *a, *b, *c;
	int rows_a, columns_a, rows_b, columns_b;
	int rank, size, chunk, count;

	double dot_prod_start_time, dot_prod_end_time;
	
	MPI_Status status;
	MPI_Request request;
	MPI_Datatype row_type, col_type;

	if(argc < 5){
		printf("ERROR! Usage: ./main rows_a column_a rows_b columns_b \n");	
		exit(1);
	}

	rows_a = atoi(argv[1]);
	columns_a = atoi(argv[2]);
	rows_b = atoi(argv[3]);
	columns_b = atoi(argv[4]);

	if (columns_a != rows_b){
		printf("ERROR! columns_a != rows_b \n");
		exit(1);
	}

	b = (double *)malloc((rows_b * columns_b) * sizeof(double));
	if (b == NULL)
		perror("Memory Allocation - [B]");

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Type_contiguous(columns_a, MPI_DOUBLE, &row_type); 
	MPI_Type_vector(rows_b, 1, columns_b, MPI_DOUBLE, &col_type); 
	MPI_Type_commit(&row_type);
	MPI_Type_commit(&col_type);

  	chunk = rows_a / size;
  		
	if (rank == 0){

		/* [A] is needed only by rank 0 because other processes will have a portion of A*/
		a = (double *)malloc((rows_a * columns_a) * sizeof(double));
		if (a == NULL)
			perror("Memory Allocation - [A]");

		init(a, b, rows_a, columns_a, rows_b, columns_b);

		for (int i = 1; i < size; i++){ 

			/* if rows of [A] cannot be equally divided among all processes */
			if (((i + 1) == size) && ((rows_a % (size) != 0))) 
				/* last processes gets all the remaining rows */ 
				count = abs(rows_a - (i * chunk));				
			else 
				count = chunk;

			/* Send chunk size, partitioned rows, [B] to the other processes*/
			MPI_Send(&count, 1, MPI_INT, i, COUNT_TAG, MPI_COMM_WORLD);
			MPI_Send(&a[i*chunk*columns_a], count, row_type, i, ROW_TAG, MPI_COMM_WORLD);
			MPI_Send(b, rows_b * columns_b, MPI_DOUBLE, i, B_TAG, MPI_COMM_WORLD);
		}

		c = (double*)malloc(rows_a * columns_b * sizeof(double));

		/* Calculate time for matrices multiplication */
		dot_prod_start_time = MPI_Wtime();	
		matrix_dot_matrix(a, b, c, chunk, columns_a, rows_b, columns_b);
		dot_prod_end_time = MPI_Wtime();

		for (int i = 1; i < size; ++i)
		{	
			MPI_Recv(&count, 1, MPI_INT, i, COUNT_TAG, MPI_COMM_WORLD, &status);
			MPI_Recv(&c[i*chunk*columns_b], count, row_type, i, C_TAG, MPI_COMM_WORLD, &status);
		}

#ifdef DEBUG
		print_matrices(a, b, c, rows_a, columns_a, rows_b, columns_b);
#endif
	}
	else if (rank > 0){

		MPI_Recv(&count, 1, MPI_INT, 0, COUNT_TAG, MPI_COMM_WORLD, &status);

		int recv_size = columns_a * count;
		a = (double*)malloc(recv_size * sizeof(double));

		MPI_Recv(a, recv_size, MPI_DOUBLE, 0, ROW_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(b, rows_b * columns_b, MPI_DOUBLE, 0, B_TAG, MPI_COMM_WORLD, &status);

		c = (double*)malloc(count * columns_b * sizeof(double));

		/* Calculate time for matrices multiplication */
		dot_prod_start_time = MPI_Wtime();	
		matrix_dot_matrix(a, b, c, count, columns_a, rows_b, columns_b);
		dot_prod_end_time = MPI_Wtime();

		/* Send to rank 0 the portion of [C] calculated */
		MPI_Send(&count, 1, MPI_INT, 0, COUNT_TAG, MPI_COMM_WORLD);
		MPI_Send(c, count, row_type, 0, C_TAG, MPI_COMM_WORLD);
	}

	double global_dot_prod_time, local_dot_prod_time;
	local_dot_prod_time = dot_prod_end_time - dot_prod_start_time;
	MPI_Reduce(&local_dot_prod_time, &global_dot_prod_time, 1, MPI_DOUBLE,MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("%d,%d,%d,%d,%.3f,%.3f,%.3f,%.3f\n",rows_a, columns_a, columns_b, size,0.0, global_dot_prod_time,0.0,global_dot_prod_time);

	free(a);
	free(b);
	free(c);
	MPI_Type_free(&row_type);
  	MPI_Type_free(&col_type);
	MPI_Finalize();

	return 0;
}
