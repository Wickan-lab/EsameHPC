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
#include "point.h"

typedef enum {QUICK, BUBBLE, SELECTION, BITONIC} sort_alg;

int OMP_ClassifyPoint(Point *dataset, Point test_point, int k,
                      int num_clusters,
                      int n, int num_threads, sort_alg alg);

void OMP_Sort(Point *arr, int n, int k, int num_threads, sort_alg alg);

#endif
