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
 * @file point.h
 * @brief Point structure definition.
 */

#ifndef  POINT_INC
#define  POINT_INC

#ifdef _MPI
#include <mpi.h>
#endif

/**
 * @brief Structure representing a point into the space.
 *
 */
typedef struct {
    int cluster_number; /** Cluster to which the point belongs */
    float x;            /** Coordinate x of the point */
    float y;            /** Coordinate y of the point */
    float distance;     /** Distance from test point */
} Point;

void Point_GeneratePoints(Point *dataset, int n, int num_threads,
                          int num_clusters);
float Point_EuclideanDistance(Point a, Point b);
#ifdef _MPI
int MPI_Type_create_Point(MPI_Datatype *point_type);
#endif
void Point_Swap(Point *a, Point *b);
void Point_Print(int iter, char *txt, Point *la, int n);
float Q_rsqrt(float number);
#endif   /* ----- #ifndef POINT_INC  ----- */
