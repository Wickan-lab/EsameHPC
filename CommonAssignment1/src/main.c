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
	@file main.c
*/

#include "dotprod.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{

	ELEMENT_TYPE *a, *b, *result;
	double time_dot = 0.0, time_init = 0.0;
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
	ENDTIME(1, time_init);

	STARTTIME(2);
	dot_product(a, b, result, rows, columns, threads);
	ENDTIME(2, time_dot);

    printf("%d;%d;%d;%f;%f\n", rows, columns, threads, time_init, time_dot);

	free(a);
	free(b);
	free(result);

	return 0;
}
