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
 * @param colums   number of columns.
 * @param threads  number of threads.
 */
void init_structures(double **a, double **b, double **result, int rows, int columns, int threads){

	double *tmp_a, *tmp_b, *tmp_result;

	#pragma omp parallel default(none) shared(tmp_a, tmp_b, tmp_result, rows, columns) num_threads(threads)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				tmp_a = (double *)malloc((rows * columns) * sizeof(double));
				if (tmp_a == NULL)
					perror("Memory Allocation - a");
	    }

	    #pragma omp section
	    {
	       	tmp_b = (double *)malloc(columns * sizeof(double));
	       	if (tmp_b == NULL)
						perror("Memory Allocation - b");
	    }

	    #pragma omp section
	    {
	    	tmp_result = (double *)malloc(rows * sizeof(double));
				if (tmp_result == NULL)
					perror("Memory Allocation - result");
	    }
		}

		#pragma omp for nowait
			for (int i = 0; i < columns; ++i)
				tmp_b[i] = 2.0;

		#pragma omp for
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				tmp_a[i*columns + j] = (double)(i + 1);
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
 * @param colums     number of columns.
 * @param threads    number of threads.
 */
void dot_product(double *a, double *b, double *result, int rows, int columns, int threads){

	int i, j;
	double dot;

	#pragma omp parallel for default(none) shared(a, b, result, rows, columns) private (i, j, dot) num_threads(threads)
		for(i = 0; i < rows; ++i){
			dot = 0.0;
			for (j = 0; j < columns; ++j)
				dot += a[i*columns + j]*b[j];
			result[i] = dot;
		}
}



/**
 * @brief This function initializes all the data structures needed in the program.
 * @param a        pointer to the matrix used in the dot product.
 * @param b        pointer to the array used in the dot product.
 * @param result   pointer to array used to store the result of dot product.
 * @param rows     number of rows.
 * @param colums   number of columns.
 * @param threads  number of threads.
 */
void init_structures_upgr(double **a, double **b, double **result, int rows, int columns, int threads){

	double *tmp_a, *tmp_b, *tmp_result;

	#pragma omp parallel default(none) shared(tmp_a, tmp_b, tmp_result, rows, columns) num_threads(threads)
	{
		
		tmp_a = (double *)malloc((rows * columns) * sizeof(double));
			if (tmp_a == NULL)
				perror("Memory Allocation - a");
	    
	    tmp_b = (double *)malloc(columns * sizeof(double));
	       	if (tmp_b == NULL)
				perror("Memory Allocation - b");
	    
	    tmp_result = (double *)malloc(rows * sizeof(double));
			if (tmp_result == NULL)
				perror("Memory Allocation - result");

		#pragma omp for nowait
			for (int i = 0; i < columns; ++i)
				tmp_b[i] = 2.0;

		#pragma omp for
		for (int i = 0; i < rows; ++i)
			for (int j = 0; j < columns; ++j)
				tmp_a[i*columns + j] = (double)(i + 1);
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
 * @param colums     number of columns.
 * @param threads    number of threads.
 */
void dot_product_upgr(double *a, double *b, double *result, int rows, int columns, int threads){

	int i, j;
	double dot;

	#pragma omp parallel for default(none) shared(a, b, result, rows, columns) private (i, j, dot) num_threads(threads)
		for(i = 0; i < rows; ++i){
			dot = 0.0;
			for (j = 0; j < columns; j+=2){
				dot += (a[i*columns + j]*b[j] + a[i*columns + (j+1)]*b[j+1]);
			}
			result[i] = dot;
		}
}