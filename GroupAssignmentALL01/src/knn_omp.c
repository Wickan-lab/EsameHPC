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
 * @file knn_omp.c
 * @brief Implementation of KNN written in C using OMP.
 *
 * Different sorting algorithms are avaliable.
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

/**
 * @brief Classify a given point using the KNN algorithm.
 *
 * @param dataset The dataset used for classification.
 * @param test_point The point to be classified.
 * @param k The number of nearest points to be considered.
 * @param n The number of points in the dataset.
 * @param num_clusters The number of clusters.
 * @param num_threads The number of threads to be used.
 * @return The resulting cluster number.
 */
int OMP_ClassifyPoint(Point *dataset, Point test_point, int k,
                      int num_clusters,
                      int n, int num_threads, sort_alg alg)
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
    }

#ifdef DEBUG
    Point_Print(-1, "Before sorting", dataset, n);
    printf("Start sorting\n");
#endif
    OMP_Sort(dataset, n, k, num_threads, alg);
    //sort(dataset, 0, n - 1, k, n, num_threads);

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
    Point_Print(-1, "After sorting", dataset, n);
#endif

    return max_pos;
}

/**
 * @brief Facade function for the selection of the sorting algorithm.
 *
 * @see sort_alg
 * @param arr Array to be sorted.
 * @param n Number of elements into the array.
 * @param k Number of neighbors to be considered.
 * @param num_threads Number of threads to use for parallelization.
 * @param alg The algorithm to use when sorting.
 */
void OMP_Sort(Point *arr, int n, int k, int num_threads, sort_alg alg)
{
    switch (alg) {
    case QUICK:
        QuickSortIterative(arr, n);
        break;
    case BUBBLE:
        BubbleSort(arr, n, num_threads);
        break;
    case SELECTION:
        SelectionSort(arr, n, k, num_threads);
        break;
    case BITONIC:
        bitonicSequenceGenerator(0, n - 1, arr, num_threads);
        break;
    default:
        fprintf(stderr, "ERROR: Sorting algorithm not valid.\n");
        break;
    }
}
