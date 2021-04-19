/* 
 * Copyright (C) 2021 - All Rights Reserved 
 *
 *	This file is part of EsameHPC.
 *
 *	EsameHPC is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * EsameHPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EsameHPC.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
	@file dotprod.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dotprod.h"

#ifdef _OPENMP
	#include <omp.h>
#else
	#define get_thread_num() 0
#endif

/**
 * @brief This function initializes all the data structures needed in the program.
 * @param a        pointer to the matrix used in the dot product.
 * @param b        pointer to the array used in the dot product.
 * @param result   pointer to array used to store the result of dot product.
 * @param rows     number of rows.
 * @param columns  number of columns.
 * @param threads  number of threads.
 */
void init_structures(ELEMENT_TYPE **a, ELEMENT_TYPE **b, ELEMENT_TYPE **result, int rows, int columns, int threads){

	ELEMENT_TYPE *tmp_a, *tmp_b, *tmp_result;

	tmp_a = (ELEMENT_TYPE *)malloc((rows * columns) * sizeof(ELEMENT_TYPE));
	if (tmp_a == NULL)
		perror("Memory Allocation - a");

   	tmp_b = (ELEMENT_TYPE *)malloc(columns * sizeof(ELEMENT_TYPE));
   	if (tmp_b == NULL)
		perror("Memory Allocation - b");

	tmp_result = (ELEMENT_TYPE *)malloc(rows * sizeof(ELEMENT_TYPE));
	if (tmp_result == NULL)
		perror("Memory Allocation - result");

	#pragma omp parallel default(none) shared(tmp_a, tmp_b, tmp_result, rows, columns) num_threads(threads)
	{
		#pragma omp for nowait
			for (int i = 0; i < columns; ++i)
				tmp_b[i] = 2.0;

		#pragma omp for
			for (int i = 0; i < rows; ++i)
				for (int j = 0; j < columns; ++j)
					tmp_a[i*columns + j] = (ELEMENT_TYPE)(i + 1);
	}

	*a = tmp_a;
	*b = tmp_b;
	*result = tmp_result;
}

/**
 * @brief This function makes the dot product between the matrix 'a' and the array 'b'.
 * @param a          pointer to the matrix used in the dot product.
 * @param b          pointer to the array used in the dot product.
 * @param result     pointer to array used to store the result of dot product.
 * @param rows       number of rows.
 * @param columns    number of columns.
 * @param threads    number of threads.
 */
void dot_product(ELEMENT_TYPE *a, ELEMENT_TYPE *b, ELEMENT_TYPE *result, int rows, int columns, int threads){

	int i, j;
	ELEMENT_TYPE dot;

	#pragma omp parallel for default(none) shared(a, b, result, rows, columns) private (i, j, dot) num_threads(threads)
		for(i = 0; i < rows; ++i){
			dot = 0.0;
			for (j = 0; j < columns; ++j)
				dot += a[i*columns + j]*b[j];
			result[i] = dot;
		}
}

/**
 * @brief This function makes the dot product between the matrix 'a' and the array 'b' using optimized for loop.
 * @param a          pointer to the matrix used in the dot product.
 * @param b          pointer to the array used in the dot product.
 * @param result     pointer to array used to store the result of dot product.
 * @param rows       number of rows.
 * @param columns     number of columns.
 * @param threads    number of threads.
 */
void dot_product_upgr(ELEMENT_TYPE *a, ELEMENT_TYPE *b, ELEMENT_TYPE *result, int rows, int columns, int threads){

	int i, j;
	ELEMENT_TYPE dot;

	#pragma omp parallel for default(none) shared(a, b, result, rows, columns, threads) private (i, j, dot) num_threads(threads)
		for(i = 0; i < rows; ++i){
			dot = 0.0;
			for (j = 0; j < columns - 2; j+=2){
				dot += (a[i*columns + j]*b[j] + a[i*columns + (j+1)]*b[j+1]);
			}
			result[i] = dot;
		}
}
