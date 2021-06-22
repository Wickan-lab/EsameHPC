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
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "TestLib.h"
#include "sort.h"
#include "point.h"
#include "knn_omp.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage : \n\t %s [nc: number of clusters] [k: number of neighbours]\n",
               argv[0]);
        exit(EXIT_FAILURE);
    }

    int N;
    int l = 9;
    printf("l : %d\n", l);
    int nc = atoi(argv[1]);
    int k = atoi(argv[2]);
    Point *dataset;
    printf("============================================\n");
    printf("============================================\n\n");
    printf("Generating points\n");
    dataset = Test_Point_GeneratePoints(dataset, l, &N, nc, make_square);
    printf("Generating points OK with N : %d\n\n", N);



    printf("Test Point is the middle of the sqaure\n\n");
    Point test_point;
    test_point.x = (float)l / 2;
    test_point.y = (float)l / 2;
    //test point is in the middle of the square

    printf("Calculating Distances\n");
    Test_Points_EuclideanDistance(dataset, N, test_point);
    printf("Calculating Distances OK\n\n");
    // Quick Sort
    printf("Testing Quick sort\n");
    QuickSortIterative(dataset, N);
    if (validate_sort(dataset, N) != 0) {
        printf("Quick Sort failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Quick sort OK\n\n");

    int test_cluster = Test_classify(dataset, N, k, nc);
    int cluster = OMP_ClassifyPoint(dataset, test_point, k, nc, N, 0, QUICK);
    printf("Test point belongs to %d\n", test_cluster);
    printf("[Classify under test] Point belongs to %d\n\n", cluster);
    assert(test_cluster == cluster);
    printf("Asserted cluster_num equality\n");

    printf("============================================\n");
    printf("============================================\n\n");

    printf("Regenerating points\n");
    dataset = Test_Point_GeneratePoints(dataset, l, &N, nc, make_square);
    printf("Regenerating points OK with N : %d\n\n", N);

    test_point.x = (float) -2 * l;
    test_point.y = (float)l / 2;

    printf("Test Point is at x : %f, y : %f\n", test_point.x, test_point.y);


    printf("Calculating Distances\n");
    Test_Points_EuclideanDistance(dataset, N, test_point);
    printf("Calculating Distances OK\n");

    //Bitonic Sort
    printf("Testing Bitonic Sort\n");
    //0, n / size - 1, local_a, 0
    bitonicSequenceGenerator(0, N, dataset, 0);
    if (validate_sort(dataset, N) != 0) {
        printf("Bitonic Sort failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Testing Bitonic Sort OK\n\n");

    test_cluster = Test_classify(dataset, N, k, nc);
    cluster = OMP_ClassifyPoint(dataset, test_point, k, nc, N, 0, BITONIC);

    printf("Test point belongs to %d\n", test_cluster);

    printf("[Classify under test] Point belongs to %d\n\n", cluster);

    assert(test_cluster == cluster);
    printf("Asserted cluster_num equality\n");

    printf("============================================\n");
    printf("============================================\n\n");

    exit(EXIT_SUCCESS);
}
