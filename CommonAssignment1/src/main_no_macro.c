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
