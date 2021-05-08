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
#include <stdio.h>
#include "utils.h"

#define FILE_A "matrix_a"
#define FILE_B "matrix_b"
#define SEQ_B "sequential_b"

int main(int argc, char**argv){
	
	if(argc != 6){
		fprintf(stderr,"Usage:\n\t%s [n_rows_A] [n_columns_A] [n_rows_B] [n_columns_B] [VERSION]\n",argv[0]);
		exit(EXIT_FAILURE);
	}	
	
	int n_rows_A    = atoi(argv[1]);
	int n_columns_A = atoi(argv[2]);
	int n_rows_B    = atoi(argv[3]);
	int n_columns_B = atoi(argv[4]);
	int version = atoi(argv[5]);
	double *a,*b;

	a = (double *)malloc((n_rows_A * n_columns_A) * sizeof(double));
	if (a == NULL)
		perror("Memory Allocation - A");
	
	b = (double *)malloc((n_rows_B * n_columns_B) * sizeof(double));
	if (b == NULL)
		perror("Memory Allocation - B");

	init(a,b,n_rows_A,n_columns_A,n_rows_B,n_columns_B);

	FILE*file_a;
	FILE*file_b;

	file_b = fopen(FILE_B,"w");
	file_a = fopen(FILE_A,"w");

	fwrite(a,sizeof(double),n_rows_A*n_columns_A,file_a);

	if (version == 5){
		fwrite(b,sizeof(double),n_rows_B*n_columns_B,file_b);
	}else{
		for (int i = 0; i < n_columns_B; i++){
			for(int j = 0; j < n_rows_B; j++){
				fwrite(b + j*n_columns_B + i,sizeof(double),1,file_b);
			}
		}		
	}

	fclose(file_b);
	fclose(file_a);
	exit(EXIT_SUCCESS);
}
