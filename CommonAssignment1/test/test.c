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
 * This file is part of CommonAssignment1.
 *
 * CommonAssignment1 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CommonAssignment1 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CommonAssignment1.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
	@file test.c
*/

#include "dotprod.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (* decorableDot)(ELEMENT_TYPE *, ELEMENT_TYPE *, ELEMENT_TYPE *, int, int, int);
typedef void (* decorableInit)(ELEMENT_TYPE **, ELEMENT_TYPE **, ELEMENT_TYPE **, int, int, int);


/**
 * @brief This function tests the initialization of all the data structures needed in the program without using sections OpenMP macro.
 * @param a        pointer to the matrix used in the dot product.
 * @param b        pointer to the array used in the dot product.
 * @param result   pointer to array used to store the result of dot product.
 * @param rows     number of rows.
 * @param columns  number of columns.
 * @param threads  number of threads.
 * @param init     decorated function init_structures.
 */
void test_init_structure(ELEMENT_TYPE **a, ELEMENT_TYPE **b, ELEMENT_TYPE **result, int rows, int columns, int threads, decorableInit init){
	init(a, b, result, rows, columns, threads);

	int i = 0;

	FILE *fp;
	fp = fopen("test_dims.txt","w");

	assert((rows*columns*sizeof(ELEMENT_TYPE)) == fwrite(*a,sizeof(ELEMENT_TYPE), rows*columns*sizeof(ELEMENT_TYPE),fp));
	assert((columns*sizeof(ELEMENT_TYPE)) == fwrite(*b,sizeof(ELEMENT_TYPE), columns*sizeof(ELEMENT_TYPE),fp));
	assert((rows*sizeof(ELEMENT_TYPE)) == fwrite(*result,sizeof(ELEMENT_TYPE), rows*sizeof(ELEMENT_TYPE),fp));

	fclose(fp);

}

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
void test_dot_product(ELEMENT_TYPE*expec,int size, ELEMENT_TYPE *a, ELEMENT_TYPE *b, ELEMENT_TYPE *result, int rows, int columns, int threads, decorableDot dot){ //double*a,double*b,int size, 
	dot(a,b,result,rows,columns,threads);

	for (int i = 0; i < size; i++){
		assert(expec[i] == (result)[i]);
	}
}


int main(int argc, char const *argv[])
{
	ELEMENT_TYPE *a, *b, *result;
	int threads = 4;
	int columns = 10;
	int rows = 10;

	test_init_structure(&a, &b, &result, rows, columns, threads, init_structures);

	ELEMENT_TYPE a1[rows*columns];
	ELEMENT_TYPE a2[rows*columns];
	ELEMENT_TYPE a3[rows*columns];
	ELEMENT_TYPE a5[rows*columns];

	ELEMENT_TYPE b1[columns];
	ELEMENT_TYPE b2[columns];
	ELEMENT_TYPE b3[columns];
	ELEMENT_TYPE b4[columns];
	ELEMENT_TYPE b5[columns];

	ELEMENT_TYPE expected_result1[] = {0,0,0,0,0,0,0,0,0,0};
	ELEMENT_TYPE expected_result2[] = {385,385,385,385,385,385,385,385,385,385};
	ELEMENT_TYPE expected_result3[] = {1,2,3,4,5,6,7,8,9,10};
	ELEMENT_TYPE expected_result4[] = {20,40,60,80,100,120,140,160,180,200};
	ELEMENT_TYPE expected_result5[] = {12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348};

	for(int i=0; i<(rows);i++){
		for(int j = 0; j < columns; j++){
			a1[i*columns + j] = (ELEMENT_TYPE)(i + 1);
			a2[i*columns + j] = (ELEMENT_TYPE)(j + 1);
			if(i == j)
				a3[i*columns + j] = (ELEMENT_TYPE)(i + 1);
			else
				a3[i*columns + j] = 0.0;
		}
	}

	for(int k = 0; k < columns; k++){
		b1[k] = 0.0;
		b2[k] = (ELEMENT_TYPE)(k + 1);
		b3[k] = 1.0;
		b4[k] = 2.0;
	}

	test_dot_product(expected_result1, rows, a1, b1, result, rows, columns, threads, dot_product);
	test_dot_product(expected_result2, rows, a2, b2, result, rows, columns, threads, dot_product);
	test_dot_product(expected_result3, rows, a3, b3, result, rows, columns, threads, dot_product);
	test_dot_product(expected_result4, rows, a1, b4, result, rows, columns, threads, dot_product);

	rows = 20;
	columns = 7;

	for(int j=0; j<(rows);j++){
		for(int z = 0; z < columns; z++){
			a5[j*columns + z] = 42;
		}
	}
	for(int t = 0; t < columns; t++){
		b5[t]= 42;
	}

	test_dot_product(expected_result5, rows, a5, b5, result, rows, columns, threads, dot_product);

	free(a);
	free(b);

	exit(EXIT_SUCCESS);
}
