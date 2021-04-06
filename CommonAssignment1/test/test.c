#include "dotprod.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define FLOAT sizeof(float)
#define DOUBLE sizeof(double)
#define INT sizeof(int)

typedef void (* decorableDot)(double *, double *, double *, int, int, int);
typedef void (* decorableInit)(double **, double **, double **, int, int, int);

void test_init_structure(double **a, double **b, double **result, int rows, int columns, int threads, decorableInit init){
	init(a, b, result, rows, columns, threads);

	int i = 0;
	//printf("check_dims of array of (hypotetical) size %f\n",(*a)[1]);
	
	FILE *fp;
	fp = fopen("test_dims.txt","w");
	
	if(!(rows*columns*DOUBLE) == fwrite(*a,DOUBLE, rows*columns*DOUBLE,fp))
		i++;
	if(!(columns*DOUBLE) == fwrite(*b,DOUBLE, columns*DOUBLE,fp))
		i++;
	if(!(rows*DOUBLE) == fwrite(*result,DOUBLE, rows*DOUBLE,fp))
		i++;

	if (!i)
		return;

	if(!(rows*columns*FLOAT) == fwrite(*a,FLOAT, rows*columns*FLOAT,fp))
		i++;
	if(!(columns*FLOAT) == fwrite(*b,FLOAT, columns*FLOAT,fp))
		i++;
	if(!(rows*FLOAT) == fwrite(*result,FLOAT, rows*FLOAT,fp))
		i++;
	
	if (!i)
		return;

	if(!(rows*columns*INT) == fwrite(*a,INT, rows*columns*INT,fp))
		i++;
	if(!(columns*INT) == fwrite(*b,INT, columns*INT,fp))
		i++;
	if(!(rows*INT) == fwrite(*result,INT, rows*INT,fp))
		i++;	
	
	fclose(fp);
	
	if(!i){
		exit(EXIT_FAILURE);
	}
}

void test_dot_product(double*expec,int size, double *a, double *b, double *result, int rows, int columns, int threads, decorableDot dot){ //double*a,double*b,int size, 
	dot(a,b,result,rows,columns,threads);
	for (int i = 0; i < size; i++){
		printf("%f %f\n",expec[i],(result)[i]);
		assert(expec[i] == (result)[i]);
	}
}


int main(int argc, char const *argv[])
{
	double *a, *b, *result;
	int threads = 4;
	int columns = 10;
	int rows = 10;
	
	test_init_structure(&a, &b, &result, rows, columns, threads, init_structures);

	double a1[rows*columns];
	double a2[rows*columns];
	double a3[rows*columns];

	double b1[columns];
	double b2[columns];
	double b3[columns];
	double b4[columns];

	double expected_result4[] = {20,40,60,80,100,120,140,160,180,200};
	double expected_result2[] = {385,385,385,385,385,385,385,385,385,385};
	double expected_result3[] = {1,2,3,4,5,6,7,8,9,10};
	double expected_result1[] = {0,0,0,0,0,0,0,0,0,0};

	for(int i=0; i<(rows);i++){
		for(int j = 0; j < columns; j++){
			a1[i*columns + j] = (double)(i + 1);
			a2[i*columns + j] = (double)(j + 1);
			if(i == j)
				a3[i*columns + j] = (double)(i + 1);
			else
				a3[i*columns + j] = 0.0;
		}
	}

	for(int k = 0; k < columns; k++){
		b1[k] = 0.0;
		b2[k] = (double)(k + 1);
		b3[k] = 1.0;
		b4[k] = 2.0;
	}

	test_dot_product(expected_result1,rows,a1,b1, result,rows,columns,threads,dot_product); //expected_result1,result,rows

	test_dot_product(expected_result2,rows,a2,b2, result,rows,columns,threads,dot_product);

	test_dot_product(expected_result3,rows,a3,b3, result,rows,columns,threads,dot_product);

	test_dot_product(expected_result4,rows,a1,b4, result,rows,columns,threads,dot_product);

	free(a);
	free(b);

	exit(EXIT_SUCCESS);

}
