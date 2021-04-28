#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void init(double **a, double **b, int rows_a, int columns_a, int rows_b, int columns_b){
	double *tmp_a, *tmp_b;

	tmp_a = (double *)malloc((rows_a * columns_a) * sizeof(double));
	if (tmp_a == NULL)
		perror("Memory Allocation - A");
	
	tmp_b = (double *)malloc((rows_b * columns_b) * sizeof(double));
	if (tmp_b == NULL)
		perror("Memory Allocation - B");
	
	for (int i = 0; i < rows_a; ++i)
		for (int j = 0; j < columns_a; ++j)
			tmp_a[i*columns_a + j] = (double)(i + 1);
	
	for (int i = 0; i < rows_b; ++i)	
		for (int j = 0; j < columns_b	; ++j)
			tmp_b[i*columns_b + j] = (double)((i + 1) * 2);
	
	*a = tmp_a;
	*b = tmp_b;
}

// *b is expected to be allocated columnwise
void matrix_dot_matrix(double *a, double *b, double *c, int n_rows_A, int n_columns_A,int n_rows_B, int n_columns_B){
	for(int i = 0; i < n_rows_A; i++){
		for(int j = 0; j < n_columns_B; j++){
			c[i*n_columns_B + j] = 0;
			for(int k = 0; k < n_rows_B; k++){
				// *b is allocated columwise!
				c[i*n_columns_B + j] += a[i*n_columns_A + k]*b[j*n_rows_B + k];
			}
		}
	}
}

void matrix_dot_matrix_seq(double *a, double *b, double *c, int n_rows_A, int n_columns_A,int n_rows_B, int n_columns_B){
	for(int i = 0; i < n_rows_A*n_columns_B; i++){
			c[i] = 0;
	}

	for(int i = 0; i < n_rows_A; i++){
		for(int k = 0; k < n_rows_B; k++){
			for(int j = 0; j < n_columns_B; j++){
				c[i*n_columns_B + j] += a[i*n_columns_A + k]*b[j + k*n_rows_B];
			}
		}
	}
}
