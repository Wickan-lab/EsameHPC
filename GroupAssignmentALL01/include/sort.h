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
#ifndef  SORT_INC
#define  SORT_INC
#include "point.h"


/*-----------------------------------------------------------------------------
 * MERGE SORT
 *-----------------------------------------------------------------------------*/
void MergeSort_merge(Point *arr, int l, int m, int r);
int merge(Point *ina, int lena, Point *inb, int lenb, Point *out);
void MergeSort(Point *arr, int l, int r, int num_threads);

/*-----------------------------------------------------------------------------
 * BUBBLE SORT
 *-----------------------------------------------------------------------------*/
void BubbleSort(Point *arr, int N, int num_threads);

/*-----------------------------------------------------------------------------
 * SELECTION SORT
 *-----------------------------------------------------------------------------*/
void SelectionSort(Point *arr, int N, int k, int num_threads);

/*-----------------------------------------------------------------------------
 * QUICK SORT
 *-----------------------------------------------------------------------------*/
int QuickSort_partition(Point data[], int left, int right);
void QuickSortIterative(Point data[], int count);

/*-----------------------------------------------------------------------------
 * BITONIC SORT
 *-----------------------------------------------------------------------------*/
void BitonicSort(Point *arr, int n);
void bitonicSequenceGenerator(int startIndex, int lastIndex, Point *ar,
                              int num_threads); // Generate a bitonic sequence from a random order
void bitonicSortFromBitonicSequence(int startIndex, int lastIndex, int dir,
                                    Point *ar); // Form a increaseing or decreasing array when a bitonic input is given to the function

#endif   /* ----- #ifndef SORT_INC  ----- */
