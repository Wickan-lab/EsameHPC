#include "dotprod.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TESTS (3)

void test_init_structure(double* a,int size){
	int i = 0;
	printf("check_dims of array of (hypotetical) size %d\n",size);
	for(i = 0; i<size;i++){}
	assert(i == size);
}

void test_dot_product(double*a,double*b,int size){
	for (int i = 0; i < size; i++){
		printf("%f %f\n",a[i],b[i]);
		assert(a[i] == b[i]);
	}
}

int main(int argc, char const *argv[])
{
	double *a, *b, *result;
	int threads = 4;
	int columns,rows = 10;
	init_structures(&a, &b, &result, rows, columns, threads);
	test_init_structure(a,(rows*columns));
	test_init_structure(b,columns);
	test_init_structure(result,rows);

	
	double a1[rows*columns];
	double a2[rows*columns];
	double a3[rows*columns];
	
	double b1[columns];
	double b2[columns];
	double b3[columns];

	double result1[rows]; 
	double result3[rows]; 
	double result2[rows]; 

	double expected_result1[] = {0,0,0,0,0,0,0,0,0,0};
	double expected_result2[] = {385,385,385,385,385,385,385,385,385,385};
	double expected_result3[] = {1,2,3,4,5,6,7,8,9,10};

	for(int i=0; i<(rows*columns);i++){
		for(int j = 0; j < columns; j++){
			a1[i*columns + j] = (double)(i + 1);
			a2[i*columns + j] = (double)(j + 1);
			if(i == j)
				a3[i*columns + j] = (double)i; 
			else
				a3[i*columns + j] = 0.0;

		}
	}

	for(int k = 0; k < columns; k++){
		b1[k] = 0.0;
		b2[k] = (double)(k + 1);
		b3[k] = 1.0;
	}
	
	dot_product(a1,b1, result1,rows,columns,threads);
	test_dot_product(expected_result1,result1,rows);

	dot_product(a2,b2,result2,rows,columns,threads);
	test_dot_product(expected_result2,result2,rows);

	dot_product(a3,b3,result3,rows,columns,threads);
	test_dot_product(result3, expected_result3,rows);

	free(a);
	free(b);

}