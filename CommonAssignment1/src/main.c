/**
	@file main.c
*/

#include "dotprod.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{

	double *a, *b, *result, timefind = 0.0;
	int rows, columns, threads;	

	if(argc < 4){
		printf("ERROR! Usage: ./main rows columns threads");	
		exit(1);
	}


	rows = atoi(argv[1]);
	columns = atoi(argv[2]);
	threads = atoi(argv[3]);

	STARTTIME(1);

	init_structures(&a, &b, &result, rows, columns, threads);
	dot_product(a, b, result, rows, columns, threads);

	ENDTIME(1,timefind);

	printf("%f\n", timefind);

	free(a);
	free(b);
	free(result);

	return 0;
}
