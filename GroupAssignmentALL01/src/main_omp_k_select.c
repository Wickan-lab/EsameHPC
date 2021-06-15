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
 * This file is part of GroupAssignment01.
 *
 * GroupAssignment01 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GroupAssignment01 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GroupAssignment01.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "knn.h"

#ifdef _OPENMP
	#include <omp.h>
#else
	#define get_thread_num() 0
#endif

int main(int argc, char const *argv[])
{	

	double time_generate = 0.0, time_classify = 0.0;

	if(argc != 6){
		fprintf(stderr,"Usage:\n\t%s [n : points to generate] [x: Coordinate x of the point to classify] [y: Coordinate y of the point to classify] [k] [num_threads]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
	int x = atoi(argv[2]);
	int y = atoi(argv[3]);
	int k = atoi(argv[4]);
	int num_threads = atoi(argv[5]);

	Point dataset[n];
	Point test_point;

	test_point.x = x;
	test_point.y = y;

	srand(time(NULL)); 

	STARTTIME(1);
	generate_points(dataset, n, num_threads);
	ENDTIME(1, time_generate);

	STARTTIME(2);
	test_point.cluster_number = classify_point_bubble(dataset, test_point, k, n, num_threads);
	ENDTIME(2, time_classify);

	printf("%d;%d;%f;%f\n", n, num_threads, time_generate, time_classify);
	//printf("Point belongs to cluster: %d\n", test_point.cluster_number);

	return 0;
}