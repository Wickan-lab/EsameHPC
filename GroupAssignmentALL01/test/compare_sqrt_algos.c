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
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h> // uint32_t
#include "point.h"

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage : #points, percentange_precision(no percentage symbol)\n");
        exit(0);
    }
    int n_points = atoi(argv[1]);
    float precision = atof(argv[2]);
    int warn_flag = 0;
    float quake = 0;
    float m_sqrt = 0;
    srand(time(NULL));
    int r;
    float upper_bound;
    float lower_bound;

    for (int i = 0; i < n_points; i++) {
        r = rand();

#ifdef DEBUG
        printf("Calculating %d\n", r);
#endif

        quake = 1 / Q_rsqrt(r);
        m_sqrt = sqrt(r);

#ifdef DEBUG
        printf("quake = %f, sqrt = %f\n", quake, m_sqrt);
#endif

        lower_bound = m_sqrt - (m_sqrt * precision / 100);
        upper_bound = m_sqrt + (m_sqrt * precision / 100);

#ifdef DEBUG
        printf("lower = %f, upper = %f\n", lower_bound, upper_bound);
#endif
        if (quake > upper_bound || quake < lower_bound) {
            printf("Precision Warning : quake = %f, sqrt = %f\n", quake, m_sqrt);
            warn_flag++;

        }

        printf("Done with precision interval of %f percent and %d precision warnings\n",
               precision, warn_flag);
    }

    if (warn_flag > 0) {
        exit(EXIT_FAILURE);
    } else {
        exit(EXIT_SUCCESS);
    }
}
