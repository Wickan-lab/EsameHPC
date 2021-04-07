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
	
	FILE *fp;
	fp = fopen("test_dims.txt","w");
	
	assert((rows*columns*DOUBLE) == fwrite(*a,DOUBLE, rows*columns*DOUBLE,fp));
	assert((columns*DOUBLE) == fwrite(*b,DOUBLE, columns*DOUBLE,fp));
	assert((rows*DOUBLE) == fwrite(*result,DOUBLE, rows*DOUBLE,fp));

	assert((rows*columns*FLOAT) == fwrite(*a,FLOAT, rows*columns*FLOAT,fp));
	assert((columns*FLOAT) == fwrite(*b,FLOAT, columns*FLOAT,fp));
	assert((rows*FLOAT) == fwrite(*result,FLOAT, rows*FLOAT,fp));	

	assert((rows*columns*INT) == fwrite(*a,INT, rows*columns*INT,fp));
	assert((columns*INT) == fwrite(*b,INT, columns*INT,fp));
	assert((rows*INT) == fwrite(*result,INT, rows*INT,fp));
	
	fclose(fp);

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
	double a5[rows*columns];

	double b1[columns];
	double b2[columns];
	double b3[columns];
	double b4[columns];
	double b5[columns];

	double expected_result1[] = {0,0,0,0,0,0,0,0,0,0};
	double expected_result2[] = {385,385,385,385,385,385,385,385,385,385};
	double expected_result3[] = {1,2,3,4,5,6,7,8,9,10};
	double expected_result4[] = {20,40,60,80,100,120,140,160,180,200};
	double expected_result5[] = {12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348,12348};

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
