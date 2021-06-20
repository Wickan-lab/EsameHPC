/*
 * Course: High Performance Computing 2020/2021
 *
 * Lecturer: Francesco Moscato  fmoscato@unisa.it
 *
 * Group:
 * Capitani Giuseppe    0622701085  g.capitani@studenti.unisa.it
 * Falanga  Armando 0622701140  a.falanga13@studenti.unisa.it
 * Terrone  Luigi       0622701071  l.terrone2@studenti.unisa.it
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of GroupAssignment01.
 *
 * GroupAssignmentALL01 is free software: you can redistribute it and/or modify
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
 * along with GroupAssignmentALL01.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
    @file knn.c
*/

#ifdef _OPENMP
#include <omp.h>
#else
#define get_thread_num() 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "knn_omp.h"
#include "point.h"
#include "sort.h"

int OMP_ClassifyPoint(Point *dataset, Point test_point, int k,
                     int n, int num_threads, void (*sort)(Point *, ...))
{
    int counter_cluster_0 = 0, counter_cluster_1 = 0;
    unsigned int *clusters_array = calloc(k, sizeof(unsigned int));
    unsigned int max_val = 0;
    unsigned int max_pos = 0;

    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (int i = 0; i < n; ++i) {
            dataset[i].distance = Point_EuclideanDistance(dataset[i], test_point);
        }

        //pass arguments in a predefined order
    }

#ifdef DEBUG
    printf("Before sorting:\n");
    for (int i = 0; i < n; ++i) {
        printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n",
               dataset[i].cluster_number, dataset[i].x, dataset[i].y, dataset[i].distance);
    }
#endif

#ifdef DEBUG
    printf("Start sorting\n");
#endif
    sort(dataset, 0, n - 1, k, n, num_threads);

    #pragma omp parallel  shared(max_val, max_pos) num_threads(num_threads)
    {

        #pragma omp critical
        {
            int clusters_index;
            for (int i = 0; i < k; i++)
            {
                clusters_index = dataset[i].cluster_number;
                clusters_array[clusters_index] += 1;
            }
        }

        #pragma omp for reduction(max:max_val,max_pos)
        for (int j = 0; j < k; j++) {
            if (max_val < clusters_array[j]) {
                max_val = clusters_array[j];
                max_pos = j;
            }

            //max_val = max_val > clusters_array[j] ? max_val : clusters_array[j];
        }
    }



#ifdef DEBUG
    for (int i = 0; i < n; ++i) {
        printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n",
               dataset[i].cluster_number, dataset[i].x, dataset[i].y, dataset[i].distance);
    }
#endif

    return max_pos;
}

/*-----------------------------------------------------------------------------
 * FACADES 
 *-----------------------------------------------------------------------------*/
void OMP_Facade_BubbleSort(Point *arr, ...)
{
    va_list list;
    va_start(list, arr);
    int start = va_arg(list, int);
    int end = va_arg(list, int);
    int k = va_arg(list, int);
    int N = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
    BubbleSort(arr, N, num_threads);
}

void OMP_Facade_SelectionSort(Point *arr, ...)
{
    va_list list;
    va_start(list, arr);
    //get 3rd argument which is k.

    int start = va_arg(list, int);//start
    int end = va_arg(list, int);//end
    int k = va_arg(list, int);
    int n = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
    SelectionSort(arr, n, k, num_threads);

}

void OMP_Facade_BitonicSort(Point *arr, ...)
{
    va_list list;
    va_start(list, arr);

    int start = va_arg(list, int);//start
    int end = va_arg(list, int);//end
    int k = va_arg(list, int);
    int n = va_arg(list, int);
    int num_threads = va_arg(list, int);
    va_end(list);
    bitonicSequenceGenerator(start, end, arr, num_threads);
}

void OMP_Facade_QuickSort(Point *arr, ...)
{
    va_list list;
    va_start(list, arr);

    int start = va_arg(list, int);//start
    int end = va_arg(list, int);//end
    int k = va_arg(list, int);
    int n = va_arg(list, int);
    va_end(list);

    QuickSortIterative(arr, n);
}

