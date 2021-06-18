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

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include "utils.h"

#define FILE_A "matrix_a"
#define FILE_B "matrix_b"
#define FILE_RES "matrix_res"

int main ( int argc, char *argv[] ){
	// Parse parameters
	if(argc != 5){
		fprintf(stderr,"Usage:\n\t%s [n_rows_A] [n_columns_A] [n_rows_B] [n_columns_B]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int n_rows_A    = atoi(argv[1]);
	int n_columns_A = atoi(argv[2]);
	int n_rows_B    = atoi(argv[3]);
	int n_columns_B = atoi(argv[4]);

	// Check matrix dimensions
	if(n_columns_A != n_rows_B){
		fprintf(stderr,"Error: n_columns_A != n_rows_B\n");
		exit(EXIT_FAILURE);
	}

	double *a,*b;

	a = (double *)malloc((n_rows_A * n_columns_A) * sizeof(double));
	if (a == NULL)
		perror("Memory Allocation - A");
	
	b = (double *)malloc((n_rows_B * n_columns_B) * sizeof(double));
	if (b == NULL)
		perror("Memory Allocation - B");

	struct tms read_start_times;
	clock_t read_start_etime;
	read_start_etime = times(&read_start_times);
	
	FILE* file_a;
	FILE* file_b;

	file_b = fopen(FILE_B,"r");
	file_a = fopen(FILE_A,"r");

	int read_b = fread(b,sizeof(double),n_rows_B*n_columns_B,file_b);
	int read_a = fread(a,sizeof(double),n_rows_A*n_columns_A,file_a);

	fclose(file_a);
	fclose(file_b);

	struct tms read_end_times;
	clock_t read_end_etime;
	read_end_etime = times(&read_end_times);

	double *c = (double*) malloc(sizeof(double) * n_rows_A * n_columns_B);

	struct tms dotprod_start_times;
	clock_t dotprod_start_etime;
	dotprod_start_etime = times(&dotprod_start_times);

	matrix_dot_matrix(a, b, c, n_rows_A, n_columns_A, n_rows_B, n_columns_B);

	struct tms dotprod_end_times;
	clock_t dotprod_end_etime;
	dotprod_end_etime = times(&dotprod_end_times);


	struct tms write_start_times;
	clock_t write_start_etime;
	write_start_etime = times(&write_start_times);

	FILE*file_c = fopen(FILE_RES,"w");
	fwrite(c,sizeof(double),n_rows_A*n_columns_B,file_c);
	fclose(file_c);

	struct tms write_end_times;
	clock_t write_end_etime;
	write_end_etime = times(&write_end_times);

#ifdef DEBUG
	for(int i = 0; i < n_rows_A; i++){
		for(int j = 0; j < n_columns_B; j++){
			printf("%.2f ",c[i*n_columns_B + j]);
		}
		putchar('\n');
	}
#endif

	// Get clock ticks per sec
	long clktck = 0;
	if((clktck = sysconf(_SC_CLK_TCK)) < 0){
		fprintf(stderr,"ERROR: failed to get clock ticks per sec\n");
		exit(EXIT_FAILURE);
	}

	double dotprod_elapsed = (dotprod_end_etime - dotprod_start_etime) / (double) clktck;
	double read_elapsed = (read_end_etime - read_start_etime) / (double) clktck;
	double write_elapsed = (write_end_etime - write_start_etime) / (double) clktck;
	double elapsed = dotprod_elapsed + read_elapsed + write_elapsed;
	printf("%d,%d,%d,%d,%.3f,%.3f,%.3f,%.3f\n",n_rows_A,n_columns_A,n_columns_B,0,read_elapsed,dotprod_elapsed,write_elapsed,elapsed);	

	free(a);
	free(b);
	free(c);
	exit(EXIT_SUCCESS);
}
