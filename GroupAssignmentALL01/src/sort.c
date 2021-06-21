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

#include <stdlib.h>
#include <math.h>
#include "point.h"

#ifdef DEBUG
#include <stdio.h>
#endif

/*-----------------------------------------------------------------------------
 * BITONIC SORT
 *-----------------------------------------------------------------------------*/

void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir,
                                    Point *ar) // Form a increaseing or decreasing array when a bitonic input is given to the function
{
    if (dir == 1) {
        int counter =
            0; // Counter to keep track of already swapped elements ,, parallelising this area results in poor performance due to overhead ,,need to fix
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2) {
            counter = 0;
            for (int i = startIndex; i + j <= lastIndex; i++) {
                if (counter < j) {
                    if (ar[i + j].distance < ar[i].distance)
                        Point_Swap(ar + i, ar + i + j);
                    counter++;
                } else {
                    counter = 0;
                    i = i + j - 1;
                }
            }
        }
    } else { // Descending sort
        int counter = 0;
        int noOfElements = lastIndex - startIndex + 1;
        for (int j = noOfElements / 2; j > 0; j = j / 2) {
            counter = 0;
            for (int i = startIndex; i <= (lastIndex - j); i++) {
                if (counter < j) {
                    if (ar[i].distance < ar[i + j].distance)
                        Point_Swap(ar + i, ar + i + j);
                    counter++;
                } else {
                    counter = 0;
                    i = i + j - 1;
                }
            }
        }
    }
}

void bitonicSequenceGenerator(int startIndex, int lastIndex, Point *ar,
                              int num_threads) // Generate a bitonic sequence from a random order
{
    int noOfElements = lastIndex - startIndex + 1;
    for (int j = 2; j <= noOfElements; j = j * 2) {
        #pragma omp parallel for num_threads(num_threads)//parallel implementation results in most performance gains here
        for (int i = 0; i < noOfElements; i = i + j) {
            if (((i / j) % 2) == 0) {
                bitonicSortFromBitonicSequence(i, i + j - 1, 1, ar);
            } else {
                bitonicSortFromBitonicSequence(i, i + j - 1, 0, ar);
            }
        }
    }
}

/*-----Non parallelizzabile per bene------*/
/*    https://courses.cs.duke.edu//fall08/cps196.1/Pthreads/bitonic.c    */
void BitonicSort(Point *arr, int N)
{

    int i, j, k;
    for (k = 2; k <= N; k = k * 2) {
        for (j = k >> 1; j > 0; j = j / 2) {
            #pragma omp parallel for
            for (i = 0; i < N; i++) {
                int ij = i ^ j;
                if ((ij) > i) {
                    if ((i & k) == 0 && arr[i].distance > arr[ij].distance)
                        Point_Swap(arr + i, arr + ij);
                    if ((i & k) != 0 && arr[i].distance < arr[ij].distance)
                        Point_Swap(arr + i, arr + ij);
                }
            }
        }
    }

}

/*-----------------------------------------------------------------------------
 * QUICK SORT
 *-----------------------------------------------------------------------------*/
int QuickSort_partition(Point data[], int left, int right)
{
    Point x = data[right];
    int i = (left - 1);

    #pragma omp parallel for
    for (int j = left; j <= right - 1; ++j) {
        if (data[j].distance <= x.distance) {
            ++i;
            Point_Swap(&data[i], &data[j]);
        }
    }

    Point_Swap(&data[i + 1], &data[right]);

    return (i + 1);
}

void QuickSortIterative(Point data[], int count)
{
    int startIndex = 0;
    int endIndex = count - 1;
    int top = -1;
    int *stack = (int *)malloc(sizeof(int) * count);

    stack[++top] = startIndex;
    stack[++top] = endIndex;

    while (top >= 0) {
        endIndex = stack[top--];
        startIndex = stack[top--];

        int p = QuickSort_partition(data, startIndex, endIndex);

        if (p - 1 > startIndex) {
            stack[++top] = startIndex;
            stack[++top] = p - 1;
        }

        if (p + 1 < endIndex) {
            stack[++top] = p + 1;
            stack[++top] = endIndex;
        }
    }

    free(stack);
}


/*-----------------------------------------------------------------------------
 * SELECTION SORT
 *-----------------------------------------------------------------------------*/
void SelectionSort(Point *arr, int N, int k, int num_threads)
{

    int min_pos;
    int k_count = 0;
    #pragma omp parallel num_threads(num_threads)
    {
        #pragma omp for
        for (int i = 0; i < k; i++) {
            min_pos = i;
            for (int j = i + 1; j < N; j++) {
                //printf("min pos: %d, swap1: %f, swap2: %f, i: %d, j: %d, N: %d\n",min_pos,arr[min_pos].distance,arr[j + 1].distance,i,j,N);
                if (fabs(arr[min_pos].distance) > fabs(arr[j].distance)) {
                    min_pos = j;
                }
            }
            Point_Swap(arr + i, arr + min_pos);
        }
    }
}

/*-----------------------------------------------------------------------------
 * MERGE SORT
 *-----------------------------------------------------------------------------*/
int merge(Point *ina, int lena, Point *inb, int lenb, Point *out)
{
#ifdef DEBUG
    printf("merge( %p , %d , %p , %d , %p )\n", ina, lena, inb, lenb, out);
#endif
    int i, j;
    int outcount = 0;

    for (i = 0, j = 0; i < lena; i++) {
        while ((inb[j].distance < ina[i].distance) && j < lenb) {
            out[outcount++] = inb[j++];
        }
        out[outcount++] = ina[i];
    }
    while (j < lenb)
        out[outcount++] = inb[j++];

    return 0;
}

/*-----------------------------------------------------------------------------
 * BUBBLE SORT
 *-----------------------------------------------------------------------------*/
void BubbleSort(Point *arr, int n, int num_threads)
{
    int isSorted = 0;

    while (!isSorted) {
        #pragma omp parallel num_threads(num_threads)
        {
            isSorted = 1;

            // Perform Bubble sort on odd indexed element
            #pragma omp parallel for
            for (int i = 1; i <= n - 2; i = i + 2)
            {
                if (arr[i].distance > arr[i + 1].distance) {
                    Point_Swap(&arr[i], &arr[i + 1]);
                    isSorted = 0;
                }
            }

            // Perform Bubble sort on even indexed element
            #pragma omp for
            for (int i = 0; i <= n - 2; i = i + 2)
            {
                if (arr[i].distance > arr[i + 1].distance) {
                    Point_Swap(&arr[i], &arr[i + 1]);
                    isSorted = 0;
                }
            }
        }
    }
}
