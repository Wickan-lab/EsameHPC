/*
 * Copyright (C) 2021 - All Rights Reserved
 *
 *	This file is part of EsameHPC.
 *
 *	EsameHPC is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 * EsameHPC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EsameHPC.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DOTPROD_H_ /* Include guard */
#define DOTPROD_H_

/** macros to get execution time: both macros have to be in the same scope
*   define a double variable to use in ENDTIME before STARTTIME:
*   double x;
*   the variable will hold the execution time in seconds.
*/

#include <time.h>

/* Token concatenation used */
#define STARTTIME(id)                           \
  clock_t start_time_42_##id, end_time_42_##id; \
  start_time_42_##id = clock()

#define ENDTIME(id, x)        \
  end_time_42_##id = clock(); \
  x = ((double)(end_time_42_##id - start_time_42_##id)) / CLOCKS_PER_SEC

void init_structures(double **, double **, double **, int, int, int);
void init_structures_upgr(double **, double **, double **, int, int, int);
void dot_product(double *, double *, double *, int, int, int);
void dot_product_upgr(double *, double *, double *, int, int, int);

#endif
