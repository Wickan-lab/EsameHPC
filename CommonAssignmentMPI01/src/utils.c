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

/**
	@file utils.c
	@brief Utility functions for CommonAssignmentMPI01.
*/


#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

/**
 * @brief This function initializes the two matrices A and B needed in the program.
 * @param a				pointer to the A matrix used in matrix dot product
 * @param b				pointer to the B matrix used in matrix dot product
 * @param rows_a		number of rows of A
 * @param columns_a	number of columns of A
 * @param rows_b		number of rows of B
 * @param columns_b	number of columns of B
 */
void init(double *a, double *b, int rows_a, int columns_a, int rows_b, int columns_b){
	for (int i = 0; i < rows_a; ++i)
		for (int j = 0; j < columns_a; ++j)
			a[i*columns_a + j] = (double)(i + 1);
	
	for (int i = 0; i < rows_b; ++i)	
		for (int j = 0; j < columns_b	; ++j)
			b[i*columns_b + j] = (double)((i + 1) * 2);
}

/**
 * @brief This function makes the dot product between the matrix A and the matrix B that is expected to be allocated columnwise.
 * @param a          pointer to the A matrix used in the dot product.
 * @param b          pointer to the B matrix used in the dot product.
 * @param c				pointer to the C matrix used to store the result of dot product.
 * @param n_rows_A		number of rows of A
 * @param n_columns_A	number of columns of A
 * @param n_rows_B		number of rows of B
 * @param n_columns_B	number of columns of B
 */
void matrix_dot_matrix(double *a, double *b, double *c, int n_rows_A, int n_columns_A,int n_rows_B, int n_columns_B){
	int sum;

	for(int i = 0; i < n_rows_A; i++){
		for(int j = 0; j < n_columns_B; j++){
			sum = 0.0;
			for(int k = 0; k < n_rows_B; k++){
				// *b is allocated columwise!
				sum += a[i*n_columns_A + k]*b[j*n_rows_B + k];
			}
			c[i*n_columns_B + j] = sum;
		}
	}
}

/**
 * @brief Prints the matrices to stdout.
 *		C should be the result of AxB.
 *
 * @param a          pointer to the A matrix used in the dot product.
 * @param b          pointer to the B matrix used in the dot product.
 * @param c				pointer to the C matrix used to store the result of dot product.
 * @param rows_a		number of rows of A
 * @param columns_a	number of columns of A
 * @param rows_b		number of rows of B
 * @param columns_b	number of columns of B
 */
void print_matrices(double *a, double *b, double *c, int rows_a, int columns_a, int rows_b, int columns_b){

	printf("Matrix A = \n");
	for (int i = 0; i < rows_a; ++i){
		for (int j = 0; j < columns_a; ++j)
			printf("%.2f ", a[i*columns_a + j]);
		printf("\n");
	}

	printf("\nMatrix B = \n");
	for (int i = 0; i < rows_b; ++i){
		for (int j = 0; j < columns_b; ++j)
			printf("%.2f ", b[i*columns_b + j]);
		printf("\n");
	}

	printf("\nMatrix C = \n");
	for (int i = 0; i < rows_a; ++i){
		for (int j = 0; j < columns_b; ++j)
			printf("%.2f ", c[i*columns_b + j]);
		printf("\n");
	}
} 

