/* 
 * Course: High Performance Computing 2020/2021 
 * 
 * Lecturer: Francesco Moscato	fmoscato@unisa.it 
 *
 * Group: 
 * Capitani	Giuseppe	0622701085	g.capitani@studenti.unisa.it 
 * Falanga	Armando	0622701140  a.falanga13@studenti.unisa.it 
 * Terrone	Luigi		0622701071  l.terrone2@studenti.unisa.it 
 *
 * Copyright (C) 2021 - All Rights Reserved 
 *
 * This file is part of CommonAssignmentMPI01. 
 *
 * CommonAssignmentMPI01 is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version. 
 *
 * CommonAssignmentMPI01 is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 *
 * You should have received a copy of the GNU General Public License 
 * along with CommonAssignmentMPI01.  If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef H_UTILS
#define H_UTILS

void init(double *a, double *b, int rows_a, int columns_a, int rows_b, int columns_b);

void matrix_dot_matrix(double *a, double *b, double *c, int n_rows_A, int n_columns_A,int n_rows_B, int n_columns_B);

void print_matrices(double *a, double *b, double *c, int rows_a, int columns_a, int rows_b, int columns_b);

#endif
