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
	@file test.c
*/


#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "utils.h"


typedef void (* decorableMM)(double *, double *, double *, int, int, int, int);


/**
 * @brief This function tests the dot product between the matrix 'a' and the array 'b'.
 * @param expec      expected dot product result.
 * @param size       size of the result.
 * @param a          pointer to the matrix used in the dot product.
 * @param b          pointer to the array used in the dot product.
 * @param result     pointer to array used to store the result of dot product.
 * @param rows       number of rows.
 * @param columns    number of columns.
 * @param threads    number of threads.
 * @param dot        decorated dot product function.
 */
void test_matrix_multiplication(double*expec,int size, double *a, double *b, double *result, int rows_A, int columns_A, int rows_B, int columns_B, decorableMM mm){
	mm(a,b,result,rows_A,columns_A,rows_B,columns_B);

	for (int i = 0; i < size; i++){
		printf("%f, %f\n",expec[i],(result)[i]);
		assert(expec[i] == (result)[i]);
	}
}


int main(int argc, char *argv[])
{
	printf("Starting\n");
	int columns_A = 5;
	int rows_A = 5;
	int columns_B = 5;
	int rows_B = 5;

	double a1[rows_A*columns_A];
	double a2[rows_A*columns_A];
	double a3[rows_A*columns_A];

	double b1[rows_B*columns_B];
	double b2[rows_B*columns_B];
	double b3[rows_B*columns_B];

	double result[rows_A*columns_B];

	double expected_result1[] = {15,40,65,90,115,30,80,130,180,230,45,120,195,270,345,60,160,260,360,460,75,200,325,450,575};
	double expected_result2[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	double expected_result3[] = {1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5};

	printf("Initialized test variables\n");

	for(int i = 0; i < rows_A; i++){
		for(int j = 0; j < columns_A; j++){
			a1[i*columns_A + j] = (double)(i + 1);
			a2[i*columns_A + j] = (double)(j + 1);
			if(i == j)
				a3[i*columns_A + j] = (double)(i + 1);
			else
				a3[i*columns_A + j] = 0.0;
		}
	}

	printf("Initialized test matrix A\n");
	for(int k = 0; k < columns_B*rows_B; k++){
		b2[k] = 0.0;
		b1[k] = (double)(k + 1);
		b3[k] = 1.0;
	}
	printf("Initialized test matrix B\n");

	test_matrix_multiplication(expected_result1, rows_A*columns_B, a1, b1, result,rows_A, columns_A, rows_B, columns_B, matrix_dot_matrix);
	test_matrix_multiplication(expected_result2, rows_A*columns_B, a2, b2, result,rows_A, columns_A, rows_B, columns_B, matrix_dot_matrix);
	test_matrix_multiplication(expected_result3, rows_A*columns_B, a3, b3, result,rows_A, columns_A, rows_B, columns_B, matrix_dot_matrix);

	printf("Tested mm ... Done");
	exit(EXIT_SUCCESS);
}
