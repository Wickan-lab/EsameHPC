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

/**
	@file knn.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "knn.h"

#ifdef _OPENMP
	#include <omp.h>
#else
	#define get_thread_num() 0
#endif

/* Function to merge the two haves arr[l..m]
 and arr[m+1..r] of array arr[] */
void merge(Point arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    Point L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    #pragma omp parallel for
	    for (i = 0; i < n1; i++)
	        L[i] = arr[l + i];

    #pragma omp parallel for
	    for (j = 0; j < n2; j++)
	        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i].distance <= R[j].distance)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements
    of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements
    of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}

 
/* l is for left index and r is
 right index of the sub-array
  of arr to be sorted */
void mergeSort(Point arr[], int l, int r)
{
   if (l < r)
   {
      // Same as (l+r)/2 but avoids
      // overflow for large l & h
      int m = l+(r-l)/2;

      #pragma omp task firstprivate (arr, l, m)
      mergeSort(arr, l, m);

      #pragma omp task firstprivate (arr, m, r)
      mergeSort(arr, m+1, r);

      #pragma omp taskwait
      merge(arr, l, m, r);
   }
}
 

float Q_rsqrt(float number)
{  
	const float x2 = number * 0.5F;
	const float threehalfs = 1.5F;

	union {
		float f;
		uint32_t i;
	} conv  = {.f = number};
	
	conv.i  = 0x5f3759df - (conv.i >> 1);
	conv.f  *= threehalfs - (x2 * conv.f * conv.f);
	
	return conv.f;
}

float euclidean_distance(Point a, Point b){
	float square_difference_x = (b.x - a.x) * (b.x - a.x);
	float square_difference_y = (b.y - a.y) * (b.y - a.y);
	float sum = square_difference_x + square_difference_y;
	return 1/Q_rsqrt(sum);
}

void generate_points(Point *dataset, int n, int num_threads){
	
	int i;
	Point new_point;

	#pragma omp parallel for default(none) shared(dataset, n) private (i, new_point) num_threads(num_threads)
	for (i = 0; i < n; ++i)
	{	
		new_point.x = rand() % 50;               /*  HARDCODED  */
		new_point.y = rand() % 50;			     /*  HARDCODED  */
		new_point.cluster_number = rand() % 2;   /*  HARDCODED  */
		dataset[i] = new_point;
	}
}
 
 
int classify_point(Point *dataset, Point test_point, int k, int n, int num_threads){
	int counter_cluster_0 = 0, counter_cluster_1 = 0;

	#pragma omp parallel num_threads(num_threads)
	{ 	
		#pragma omp for
		for (int i = 0; i < n; ++i)
		{
			dataset[i].distance = euclidean_distance(dataset[i], test_point);
		}

		#pragma omp single
		QuickSortIterative(dataset, n);
		
		
		#pragma omp for reduction(+:counter_cluster_0, counter_cluster_1)
			for (int i = 0; i < k; ++i)
			{
				if (dataset[i].cluster_number == 0)
					counter_cluster_0 += 1;
				else
					counter_cluster_1 += 1;
			}
	}

	/*for (int i = 0; i < n; ++i)
	{
		printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n", dataset[i].cluster_number, dataset[i].x, dataset[i].y, dataset[i].distance);
	}*/
	
	return (counter_cluster_1 >= counter_cluster_0) ? 1 : 0; 
}


void Swap(Point *a, Point *b)
{
	Point temp = *a;
	*a = *b;
	*b = temp;
}

int Partition(Point data[], int left, int right)
{
	Point x = data[right];
	int i = (left - 1);

	#pragma omp parallel for 
	for (int j = left; j <= right - 1; ++j)
	{
		if (data[j].distance <= x.distance)
		{
			++i;
			Swap(&data[i], &data[j]);
		}
	}

	Swap(&data[i + 1], &data[right]);

	return (i + 1);
}

void QuickSortIterative(Point data[], int count) {
	int startIndex = 0;
	int endIndex = count - 1;
	int top = -1;
	int* stack = (int*)malloc(sizeof(int) * count);

	stack[++top] = startIndex;
	stack[++top] = endIndex;

	while (top >= 0)
	{
		endIndex = stack[top--];
		startIndex = stack[top--];

		int p = Partition(data, startIndex, endIndex);

		if (p - 1 > startIndex)
		{
			stack[++top] = startIndex;
			stack[++top] = p - 1;
		}

		if (p + 1 < endIndex)
		{
			stack[++top] = p + 1;
			stack[++top] = endIndex;
		}
	}

	free(stack);
}