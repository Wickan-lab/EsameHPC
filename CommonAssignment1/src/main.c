/**
	@file main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _OPENMP
	#include <omp.h>
#else 
	#define get_thread_num() 0
#endif

/** macros to get execution time: both macros have to be in the same scope
*   define a double variable to use in ENDTIME before STARTTIME:
*   double x;
*   the variable will hold the execution time in seconds.
*/

#define STARTTIME(id) clock_t start_time_42_id,end_time_42_id; start_time_42_id = clock()

#define ENDTIME(id,x) end_time_42_id=clock(); x=((double) (end_time_42_id-start_time_42_id)) /  CLOCKS_PER_SEC

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

int main(int argc, char const *argv[])
{	

	double *a, *b, *result, timefind = 0.0;
	int rows, columns, threads;	

	if(argc < 4){
		printf("ERROR! Usage: ./main rows columns threads");	
		exit(1);
	}

	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
	threads = atoi(argv[3]);

	STARTTIME(1);

	init_structures(&a, &b, &result, rows, columns, threads);
	dot_product(a, b, result, rows, columns, threads);

	ENDTIME(1,timefind);

	printf("Time : %f\n", timefind);

	return 0;
}