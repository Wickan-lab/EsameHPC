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

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

#define NDIMS 2
#define ARG_A_ROWS 1
#define ARG_A_COLUMNS 2
#define ARG_B_ROWS 1
#define ARG_B_COLUMNS 2

#define FILENAME_A "matrix_a"
#define FILENAME_B "matrix_b"
#define FILENAME_C "matrix_c"

int main (int argc, char * argv[])
{
	MPI_Init(&argc, &argv); //init MPI environment

	// Parse command line parameters
	if(argc != 5){
		MPI_Finalize(); //Terminate MPI execution env.
		exit(EXIT_FAILURE);
	}
	int len_a_colum = atoi(argv[ARG_A_ROWS]);
	int len_a_row = atoi(argv[ARG_A_COLUMNS]);
	int len_b_colum = atoi(argv[ARG_B_ROWS]);
	int len_b_row = atoi(argv[ARG_B_COLUMNS]);

	// Get number of spawned processes as
	// world communicator size.
	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// Get optimal dims for cartesian decomposition
	int dims[NDIMS] = {0,0};
	if(MPI_Dims_create(size,NDIMS,dims) != MPI_SUCCESS){
		MPI_Finalize(); //Terminate MPI execution env.
		exit(EXIT_FAILURE);
	}

	// Perform cartesian decomposition
	MPI_Comm cartComm;
	int periods[NDIMS] = {true, false};
	int reorder = false;
	if(MPI_Cart_create(MPI_COMM_WORLD,NDIMS,dims,periods,reorder,&cartComm) != MPI_SUCCESS){
		MPI_Finalize(); //Terminate MPI execution env.
		exit(EXIT_FAILURE);
	}
	
	// Get this process' rank and coordinates
	int rank, coords[NDIMS];
	MPI_Comm_rank(cartComm,&rank);
	MPI_Cart_coords(cartComm,rank,NDIMS,coords);

#ifdef DEBUG
	if(rank == 0)printf("dims = { %d , %d }\n",dims[0],dims[1]);	
#endif

	// Crate sub matrix datatype for C matrix based on process' coordinates
	
	//		Full matrix sizes
	int array_of_sizes[NDIMS];
	array_of_sizes[0] = len_a_colum;
	array_of_sizes[1] = len_b_row;
	
	//		Sub-matrix coordinates
	int array_of_starts[NDIMS];
	array_of_starts[0] = coords[0] * ( len_a_colum / dims[0] );
	array_of_starts[1] = coords[1] * ( len_b_row / dims[1] );
	
	//		Sub-matrix sizes, pay attention to division remainder
	int array_of_subsizes[NDIMS];
	if(coords[0] != ( dims[0] - 1 ) ){
		array_of_subsizes[0] =  len_a_colum / dims[0];
	} else {
		array_of_subsizes[0] = len_a_colum - array_of_starts[0];
	}
	if(coords[1] != ( dims[1] - 1 ) ){
		array_of_subsizes[1] =  len_b_row / dims[1];
	} else {
		array_of_subsizes[1] = len_b_row - array_of_starts[1];
	}
	
	//		Datatype creation and commit
	MPI_Datatype mpidt_c_sub_matrix;
	MPI_Type_create_subarray(NDIMS,array_of_sizes,array_of_subsizes,array_of_starts,MPI_ORDER_C,MPI_DOUBLE,&mpidt_c_sub_matrix);
	MPI_Type_commit(&mpidt_c_sub_matrix);

	// Create sub matrix datatype for A matrix (Allocated rowwise)
	MPI_Datatype mpidt_a_sub_matrix;
	int len_a_rows_chunk = array_of_subsizes[0];
	MPI_Type_contiguous(len_a_rows_chunk*len_a_row,MPI_DOUBLE,&mpidt_a_sub_matrix);
	MPI_Type_commit(&mpidt_a_sub_matrix);

	// Read A chunk from file
	
	//		Open A file
	MPI_File file_a;
	MPI_File_open(MPI_COMM_WORLD, FILENAME_A, MPI_MODE_RDONLY, MPI_INFO_NULL, &file_a);

	//		Compute displacement and set view
	MPI_Offset displacement_file_a = coords[0] * ( len_a_colum / dims[0] ) \
												* len_a_row * sizeof(double);
	MPI_File_set_view(file_a, displacement_file_a, MPI_DOUBLE, \
			mpidt_a_sub_matrix,"native",MPI_INFO_NULL);

	//		Allocate A sub matrix	
	double *a = (double*)malloc(len_a_rows_chunk*len_a_row*sizeof(double));

	//		Read A from file
	MPI_File_read(file_a, a, 1, mpidt_a_sub_matrix,MPI_STATUS_IGNORE);

	//		Close file
	MPI_File_close(&file_a);

#ifdef DEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	for(int k = 0; k < size; k++){
		if(rank == k){
			printf("Rank %d - A matrix\n",rank);
			for(int i = 0; i < len_a_rows_chunk; i++){
				for(int j = 0; j < len_a_row; j++){
					printf("%.2f ",a[i*len_a_row + j]);
				}
				putchar('\n');
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
#endif

	// Create sub matrix datatype for B matrix (Allocated columnwise)
	MPI_Datatype mpidt_b_sub_matrix;
	int len_b_columns_chunk = array_of_subsizes[1];
	MPI_Type_contiguous(len_b_colum*len_b_columns_chunk,MPI_DOUBLE,&mpidt_b_sub_matrix);
	MPI_Type_commit(&mpidt_b_sub_matrix);

	// Read B chunk from file
	
	//		Open B file
	MPI_File file_b;
	MPI_File_open(MPI_COMM_WORLD, FILENAME_B, MPI_MODE_RDONLY, MPI_INFO_NULL, &file_b);

	//		Compute displacement and set view
	MPI_Offset displacement_file_b = coords[1] * ( len_b_row / dims[1] ) * \
												len_b_colum * sizeof(double);
	MPI_File_set_view(file_b, displacement_file_b, MPI_DOUBLE, \
			mpidt_b_sub_matrix,"native",MPI_INFO_NULL);

	//		Allocate B sub matrix	
	double *b = (double*)malloc(len_b_colum * len_b_columns_chunk * sizeof(double));

	//		Read B from file
	MPI_File_read(file_b, b, 1, mpidt_b_sub_matrix,MPI_STATUS_IGNORE);

	//		Close file
	MPI_File_close(&file_b);

#ifdef DEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	for(int k = 0; k < size; k++){
		if(rank == k){
			printf("Rank %d - B matrix\n",rank);
			for(int i = 0; i < len_b_colum; i++){
				for(int j = 0; j < len_b_columns_chunk; j++){
					printf("%.2f ",b[j*len_b_colum + i]);
				}
				putchar('\n');
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
#endif

#ifdef DEBUG
	printf("Rank %d: A is %dx%d , B is %dx%d\n",rank,len_a_rows_chunk,len_a_row, \
			len_b_row,len_b_columns_chunk);
#endif
	// Calculate dot product

	//		Allocate C sub matrix
	double *c = (double*)malloc(len_a_rows_chunk * len_b_columns_chunk * sizeof(double));

	double start = MPI_Wtime();
	//		Perform dot product
	matrix_dot_matrix( a, b, c, len_a_rows_chunk, len_a_row, len_b_colum, len_b_columns_chunk);
	double end = MPI_Wtime();

	double send = end - start ,recv;
	MPI_Reduce(&send,&recv,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
	recv /= size;

#ifdef DEBUG
	printf("Rank %d: %dx%d %.2f\n",rank,len_a_rows_chunk,len_b_columns_chunk,end-start);
	if(rank == 0) printf("Mean: %.2f\n",recv);
#endif

	// Write C sub-matrix on file
	MPI_File file_c;
	MPI_File_open(MPI_COMM_WORLD, FILENAME_C, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &file_c);
	
	//		Set view
	MPI_File_set_view(file_c, 0, MPI_DOUBLE, mpidt_c_sub_matrix, "native", MPI_INFO_NULL);

	//		Write C sub-matrix on file
	MPI_File_write(file_c, c, len_a_rows_chunk * len_b_columns_chunk, MPI_DOUBLE, MPI_STATUS_IGNORE);

#ifdef DEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	for(int k = 0; k < size; k++){
		if(rank == k){
			printf("Rank %d - C matrix\n",rank);
			for(int i = 0; i < len_a_rows_chunk; i++){
				for(int j = 0; j < len_b_columns_chunk; j++){
					printf("%.2f ",c[i*len_b_columns_chunk + j]);
				}
				putchar('\n');
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}
#endif

#ifdef DEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_set_view(file_c,0,MPI_BYTE,MPI_BYTE,"native",MPI_INFO_NULL);
	if(rank == 0){
		printf("Total\n");
		double *c = (double*)malloc(len_a_colum * len_b_row * sizeof(double));
		MPI_File_read(file_c, c, len_a_colum * len_b_row, MPI_DOUBLE, MPI_STATUS_IGNORE);
		for(int i = 0; i < len_a_colum; i++){
			for(int j = 0; j < len_b_row; j++){
				printf("%.2f ",c[i*len_b_row + j]);
			}
			putchar('\n');
		}
		free(c);
		MPI_File_close(&file_c);
	}
#endif

	//		Close C file
	MPI_File_close(&file_c);

	free(a);
	free(b);
	free(c);
	MPI_Type_free(&mpidt_b_sub_matrix);
	MPI_Type_free(&mpidt_a_sub_matrix);
	MPI_Type_free(&mpidt_c_sub_matrix);
	MPI_Finalize(); //Terminate MPI execution env.
	exit(EXIT_SUCCESS);
}

