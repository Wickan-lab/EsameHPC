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
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <float.h>

#ifdef _OPENMP
	#include <omp.h>
#else
	#define get_thread_num() 0
#endif


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
	
	srand(time(NULL)); 

	#pragma omp parallel for default(none) shared(dataset, n) private (i, new_point) num_threads(num_threads)
	for (i = 0; i < n; ++i)
	{	
		new_point.x = rand() % 300;               /*  HARDCODED  */
		new_point.y = rand() % 300;			     /*  HARDCODED  */
		new_point.cluster_number = rand() % 2;   /*  HARDCODED  */
		dataset[i] = new_point;
	}
}

int classify_point_no_conflict(Point *dataset, Point test_point, int k, int n, int num_threads, void (*sort)(Point*,...)){
	int counter_cluster_0 = 0, counter_cluster_1 = 0;
	#pragma omp parallel num_threads(num_threads)
    {
    	#pragma omp for
    		for (int i = 0; i < n; ++i)
    		{
    			dataset[i].distance = euclidean_distance(dataset[i], test_point);
    		}

        //pass arguments in a predefined order
    }

	
#ifdef DEBUG
	 printf("Start sorting\n");
#endif
    sort(dataset, 0, n - 1, k, n, num_threads);

	#pragma omp parallel num_threads(num_threads)
	{ 	
        #pragma omp for reduction(+:counter_cluster_0, counter_cluster_1)
        for (int i = 0; i < k; ++i)
			{
            if (dataset[i].cluster_number == 0)
               counter_cluster_0 += 1;
            else
               counter_cluster_1 += 1;
         }
	}
#ifdef DEBUG
	for (int i = 0; i < n; ++i)
	{
		printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n", dataset[i].cluster_number, dataset[i].x, dataset[i].y, dataset[i].distance);
	}
#endif
	
	return (counter_cluster_1 >= counter_cluster_0) ? 1 : 0; 
}

void swap(Point*arr, int i, int j){
    Point temp;
    temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void facade_mergeSort(Point*arr, ...){
    va_list list;
    va_start(list, arr); 
    //get start and end
    int start = va_arg(list, int);
    int end = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
    mergeSort(arr, start, end, num_threads);
}

void facade_bubble_sort(Point*arr, ...){
    va_list list;
    va_start(list, arr); 
    int start = va_arg(list, int);
    int end = va_arg(list, int);
    int k = va_arg(list, int);
    int N = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
    bubble_sort(arr, N, num_threads);
}

void facade_k_selection_sort(Point*arr, ...){
    va_list list;
    va_start(list, arr); 
    //get 3rd argument which is k.
    
    int start = va_arg(list, int);//start
    int end = va_arg(list, int);//end
    int k = va_arg(list, int);
    int n = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
    k_selection_sort(arr, n, k, num_threads);
    
}

void facade_bitonicSort(Point* arr, ...){
    va_list list;
    va_start(list, arr); 
    
    int start = va_arg(list, int);//start
    int end = va_arg(list, int);//end
    int k = va_arg(list, int);
    int n = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
	 bitonicSequenceGenerator(start,end,arr,num_threads); 
}

void facade_quicksort(Point *arr, ...){
   va_list list;
   va_start(list,arr);

   int start = va_arg(list, int);//start
   int end = va_arg(list, int);//end
   int k = va_arg(list, int);
   int n = va_arg(list, int);
   va_end(list);

	QuickSortIterative(arr,n);
}

void k_selection_sort(Point*arr, int N, int k, int num_threads){
    
    int min_pos;
    int k_count = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for(int i = 0; i < k; i++){
            min_pos = i;
            for(int j = i + 1; j < N; j++){
                //printf("min pos: %d, swap1: %f, swap2: %f, i: %d, j: %d, N: %d\n",min_pos,arr[min_pos].distance,arr[j + 1].distance,i,j,N);
                if(fabs(arr[min_pos].distance) > fabs(arr[j].distance)){
                    min_pos = j;
                }
            }
            swap(arr,i,min_pos);
        }
    }
}

/* Function to merge the two haves arr[l..m]
 and arr[m+1..r] of array arr[] */
void omp_merge(Point arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    Point L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    #pragma omp for nowait
        for (i = 0; i < n1; i++)
            L[i] = arr[l + i];

    #pragma omp for
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
void mergeSort(Point arr[], int l, int r, int num_threads)
{

    #pragma omp parallel num_threads(num_threads)
    {
        if (l < r)
        {
          // Same as (l+r)/2 but avoids
          // overflow for large l & h
          int m = l+(r-l)/2;

          #pragma omp task firstprivate (arr, l, m)
          mergeSort(arr, l, m, num_threads);

          #pragma omp task firstprivate (arr, m, r)
          mergeSort(arr, m+1, r, num_threads);

          #pragma omp taskwait
          omp_merge(arr, l, m, r);
        }
    }   
}

void bubble_sort(Point*arr, int n, int num_threads)
{
    int isSorted = 0; 
  
    while (!isSorted) {
        #pragma omp parallel num_threads(num_threads)
        {
            isSorted = 1;
  
            // Perform Bubble sort on odd indexed element
            #pragma omp parallel for 
            for (int i = 1; i <= n - 2; i = i + 2) {
                if (arr[i].distance > arr[i + 1].distance) {
                    Swap(&arr[i], &arr[i + 1]);
                    isSorted = 0;
                }
            }
      
            // Perform Bubble sort on even indexed element
            #pragma omp for 
            for (int i = 0; i <= n - 2; i = i + 2) {
                if (arr[i].distance > arr[i + 1].distance) {
                    Swap(&arr[i], &arr[i + 1]);
                    isSorted = 0;
                }
            }
        }
    }
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


void ascendingSwap(int index1, int index2, Point *ar) // Swap two values such that they appear in ascending order in the array
{
    if (ar[index2].distance < ar[index1].distance)
    {
        Point temp = ar[index2];
        ar[index2] = ar[index1];
        ar[index1] = temp;
    }
}

void decendingSwap(int index1, int index2, Point *ar) // Swap two values such that they appear in decending order in the array
{
    if (ar[index1].distance < ar[index2].distance)
    {
        Point temp = ar[index2];
        ar[index2] = ar[index1];
        ar[index1] = temp;
    }
}

void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir, Point *ar) // Form a increaseing or decreasing array when a bitonic input is given to the function
{
    if (dir == 1)
    {
        int counter = 0; // Counter to keep track of already swapped elements ,, parallelising this area results in poor performance due to overhead ,,need to fix
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2)
        {
            counter = 0;
            for (int i = startIndex; i + j <= lastIndex; i++)
            {
                if (counter < j)
                {
                    ascendingSwap(i, i + j, ar);
                    counter++;
                }
                else
                {
                    counter = 0;
                    i = i + j - 1;
                }
            }
        }
    }
    else // Descending sort
    {
        int counter = 0;
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2)
        {
            counter = 0;
            for (int i = startIndex; i <= (lastIndex - j); i++)
            {
                if (counter < j)
                {
                    decendingSwap(i, i + j, ar);
                    counter++;
                }
                else
                {
                    counter = 0;
                    i = i + j - 1;
                }
            }
        }
    }
}
void bitonicSequenceGenerator(int startIndex, int lastIndex, Point*ar,int num_threads) // Generate a bitonic sequence from a random order
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2)
    {
        #pragma omp parallel for num_threads(num_threads)//parallel implementation results in most performance gains here
        for (int i = 0; i < noOfElements; i = i + j)
        {
            if (((i / j) % 2) == 0)
            {
                bitonicSortFromBitonicSequence(i, i + j - 1, 1, ar);
            }
            else
            {
                bitonicSortFromBitonicSequence(i, i + j - 1, 0, ar);
            }
        }
    }
}

/*    https://courses.cs.duke.edu//fall08/cps196.1/Pthreads/bitonic.c    */
void BitonicSort(Point* arr, int N, int num_threads) {

  int i,j,k;
      for (k=2; k<=N; k = k*2) {
        for (j=k>>1; j>0; j = j / 2) {
			#pragma omp parallel for
          for (i=0; i<N; i++) {
					int ij=i^j;
        			if ((ij)>i) {
        			  if ((i&k)==0 && arr[i].distance > arr[ij].distance) 
        			      swap(arr,i,ij);
        			  if ((i&k)!=0 && arr[i].distance < arr[ij].distance)
        			      swap(arr,i,ij);
        			}
          }
        }
      }

}
