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

#ifndef KNN_H_ 
#define KNN_H_ 

#include <time.h>

/** macros to get execution time: both macros have to be in the same scope
*   define a double variable to use in ENDTIME before STARTTIME:
*   double x;
*   the variable will hold the execution time in seconds.
*/

/* Token concatenation used */
#define STARTTIME(id)                           \
  clock_t start_time_42_##id, end_time_42_##id; \
  start_time_42_##id = clock()

#define ENDTIME(id, x)        \
  end_time_42_##id = clock(); \
  x = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC


typedef struct 
{	
	int cluster_number;   /* Cluster to which the point belongs */
	float x, y; 	      /* Coordinate x and y of the point    */
	float distance; 	  /* Distance from test point           */
} Point;


float Q_rsqrt(float number);
float euclidean_distance(Point a, Point b);
void generate_points(Point *dataset, int n, int num_threads, int num_clusters);
int classify_point_no_conflict(Point *dataset, Point test_point, int k, int n, int num_threads,void (*sort)(Point*,...));
void swap(Point*arr, int i, int j);

void merge(Point* arr, int l, int m, int r);
void mergeSort(Point* arr, int l, int r, int num_threads);
void bubble_sort(Point*arr,int N, int num_threads);
void k_selection_sort(Point*arr, int N, int k, int num_threads);
void BitonicSort(Point*arr, int n, int num_threads);

void facade_k_selection_sort(Point*arr, ...);
void facade_bubble_sort(Point*arr, ...);
void facade_mergeSort(Point*arr, ...);
void facade_quicksort(Point*arr,...);
void facade_bitonicSort(Point*arr,...);
void Swap(Point *a, Point *b);
int Partition(Point data[], int left, int right);
void QuickSortIterative(Point data[], int count);
void bitonicSequenceGenerator(int startIndex, int lastIndex, Point*ar,int num_threads); // Generate a bitonic sequence from a random order

#endif
