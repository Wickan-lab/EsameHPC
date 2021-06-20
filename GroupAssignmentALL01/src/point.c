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

#include "point.h"
#include <stdint.h>
#include <stdio.h>

#ifdef _MPI
#include <mpi.h>
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

void Point_GeneratePoints(Point *dataset, int n, int num_threads,
                        int num_clusters)
{
    int i;
    Point new_point;


    #pragma omp parallel for default(none) shared(dataset, n, num_clusters) private (i, new_point) num_threads(num_threads)
    for (i = 0; i < n; ++i) {
        new_point.x = (i + 3.5) * 2.43;           /*  HARDCODED  */
        new_point.y = (i + 2.1) * 3.6;           /*  HARDCODED  */
        new_point.cluster_number = ((int)((i + 1.7) * 5.49)) %
                                   num_clusters; /*  HARDCODED  */
        dataset[i] = new_point;
    }
}
float Point_EuclideanDistance(Point a, Point b)
{
    float square_difference_x = (b.x - a.x) * (b.x - a.x);
    float square_difference_y = (b.y - a.y) * (b.y - a.y);
    float sum = square_difference_x + square_difference_y;
    return 1 / Q_rsqrt(sum);
}

void Point_Swap(Point *a, Point *b)
{
    Point temp = *a;
    *a = *b;
    *b = temp;
}

void Point_Print(int iter, char *txt, Point *la, int n)
{
	int rank = 0;
#ifdef _MPI
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
#endif
    printf("[%d] %s iter %d: >\n", rank, txt, iter);
    for (int j = 0; j < n; j++)
        printf("Cluster = %d -- x = %.2f -- y = %.2f -- distance = %.2f\n", \
               la[j].cluster_number, la[j].x, la[j].y, la[j].distance);
}

#ifdef _MPI
int MPI_Type_create_Point(MPI_Datatype *point_type)
{
    int lengths[2] = { 1, 3 };

    MPI_Aint displacements[2];
    Point dummy_point;
    MPI_Aint base_address;
    MPI_Get_address(&dummy_point, &base_address);
    MPI_Get_address(&dummy_point.cluster_number, &displacements[0]);
    MPI_Get_address(&dummy_point.x, &displacements[1]);
    displacements[0] = MPI_Aint_diff(displacements[0], base_address);
    displacements[1] = MPI_Aint_diff(displacements[1], base_address);
    MPI_Datatype types[2] = { MPI_INT, MPI_FLOAT };
    return MPI_Type_create_struct(2, lengths, displacements,
                                  types, point_type);
}
#endif
