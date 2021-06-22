/*
 * Course: High Performance Computing 2020/2021
 *
 * Lecturer: Francesco Moscato  fmoscato@unisa.it
 *
 * Group:
 * Capitani Giuseppe    0622701085  g.capitani@studenti.unisa.it
 * Falanga  Armando     0622701140  a.falanga13@studenti.unisa.it
 * Terrone  Luigi       0622701071  l.terrone2@studenti.unisa.it
 *
 * Copyright (C) 2021 - All Rights Reserved
 *
 * This file is part of GroupAssignmentALL01.
 *
 * GroupAssignmentALL01 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * GroupAssignmentALL01 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GroupAssignmentALL01.  If not, see <http://www.gnu.org/licenses/>.
 */
/** 
 * @file TestLib.c
 * @brief Utility functions for testing.
 */
#include "point.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Check if an array of `Point` is sorted by `distance`.
 *
 * @param dataset array of `Point`
 * @param n number of elements in `dataset`
 * @return 0 if the array is sorted, -1 otherwise
 */
int validate_sort(Point *dataset, int n)
{
    for (int i = 0; i < n - 1; i++) {
        if (dataset[i].distance == 0)
            fprintf(stderr, "[Validate Sort] WARNING: Element at index %d is 0\n", i);

        if (dataset[i].distance > dataset[i + 1].distance) {
            printf("[Validate Sort] Error in sort index %d and %d & elem1: %f elem2: %f\n",
                   i, i + 1, dataset[i].distance, dataset[i + 1].distance);
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Creates a dataset of `Point` representing the four vertices of a square.
 *
 * @param dataset array of `Point`
 * @param l length of the side
 * @param nc number of clusters
 * @param n number of vertices generated
 */
Point *make_square(Point *dataset, int l, int nc, int *n)
{

    int N = 4;
    dataset = calloc(N, sizeof(Point));
    printf("Allocated Dataset Space\n");

    for (int i = 0; i < N; i++) {
        dataset[i].x = ((i % 2) == 0) ? 0 : l;   //0 l 0 l
        dataset[i].y = (i < N / 2) ? 0 : l;     //0 0 l l
        dataset[i].cluster_number = rand() % nc;
        printf("Point %f %f of cluster %d\n", dataset[i].x, dataset[i].y,
               dataset[i].cluster_number);
    }
    *n = N;
    return dataset;
}

/**
 * @brief Generate an array of `Point` using a generation function passed as argument.
 *
 * @param dataset array of `Point`
 * @param l parameter for the shape
 * @param n number of `Point` generated
 * @param nc number of clusters
 * @return the `dataset` pointer
 */
Point *Test_Point_GeneratePoints(Point *dataset, int l, int *n, int nc,
                                 Point * (*shape)(Point *, int, int, int *))
{
    dataset = shape(dataset, l, nc, n);
    return dataset;
}

/** 
 * @brief Print euclidean distances between points of the dataset and the test point.
 *
 * @param dataset array of `Point`
 * @param N number of point in `dataset`
 * @param test_point test point for distance calculation
 */
void Test_Points_EuclideanDistance(Point *dataset, int N, Point test_point)
{
    for (int i = 0; i < N; i++) {
        dataset[i].distance = Point_EuclideanDistance(dataset[i], test_point);
        printf("Distance from test point is %f @index : %d\n", dataset[i].distance,
               i);
    }
}

/**
 * @brief Classify the test point based on the ordered dataset.
 *
 * @param ordered_dataset array of ordered `Point`
 * @param N number of elements contained in `ordered_dataset`
 * @param k number of neighbors to consider
 * @param nc number of clusters
 */
int Test_classify(Point *ordered_dataset, int N, int k, int nc)
{
    if (nc > N) {
        printf("Number of clusters more than size\n");
        exit(EXIT_FAILURE);
    }

    int *clusters = calloc(nc, sizeof(int));
    printf("Counting knn points cluster_number\n");
    for (int i = 0; i < k; i++) {
        clusters[ordered_dataset[i].cluster_number]++;
        printf("Considering point with x: %f, y: %f, cluster: %d\n",
               ordered_dataset[i].x, ordered_dataset[i].y,
               ordered_dataset[i].cluster_number);
    }
    printf("Counting knn points cluster_number OK\n");

    printf("Calculating most frequent cluster_number\n");
    int pos = 0, max = 0;
    for (int i = 0; i < k; i++) {
        if (max < clusters[i]) {
            max = clusters[i];
            pos = i;
        }
    }

    printf("Calculating most frequent cluster_number OK\n");
    return pos;
}
