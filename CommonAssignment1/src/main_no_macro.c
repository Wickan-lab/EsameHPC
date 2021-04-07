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

/**
	@file main_no_macro.c
*/

#include "dotprod.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{

	double *a, *b, *result;
	int rows, columns, threads;

	if(argc < 4){
		printf("ERROR! Usage: ./main rows columns threads");
		exit(1);
	}

	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
	threads = atoi(argv[3]);

	init_structures(&a, &b, &result, rows, columns, threads);
	dot_product_upgr(a, b, result, rows, columns, threads);

	free(a);
	free(b);
	free(result);

	return 0;
}
