#include	<stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/times.h>
#include "utils.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] ){
	// Parse parameters
	if(argc != 5){
		fprintf(stderr,"Usage:\n\t%s [n_rows_A] [n_columns_A] [n_rows_B] [n_columns_B]\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	int n_rows_A    = atoi(argv[1]);
	int n_columns_A = atoi(argv[2]);
	int n_rows_B    = atoi(argv[3]);
	int n_columns_B = atoi(argv[4]);

	// Check matrix dimensions
	if(n_columns_A != n_rows_B){
		fprintf(stderr,"Error: n_columns_A != n_rows_B\n");
		exit(EXIT_FAILURE);
	}

	double *a,*b;
	init(&a,&b,n_rows_A,n_columns_A,n_rows_B,n_columns_B);

	double *c = (double*) malloc(sizeof(double) * n_rows_A * n_columns_B);

	struct tms dotprod_start_times;
	clock_t dotprod_start_etime;
	dotprod_start_etime = times(&dotprod_start_times);

	matrix_dot_matrix_seq(a, b, c, n_rows_A, n_columns_A, n_rows_B, n_columns_B);

	struct tms dotprod_end_times;
	clock_t dotprod_end_etime;
	dotprod_end_etime = times(&dotprod_end_times);

#ifdef DEBUG
	for(int i = 0; i < n_rows_A; i++){
		for(int j = 0; j < n_columns_B; j++){
			printf("%.2f ",c[i*n_columns_B + j]);
		}
		putchar('\n');
	}
#endif

	// Get clock ticks per sec
	long clktck = 0;
	if((clktck = sysconf(_SC_CLK_TCK)) < 0){
		fprintf(stderr,"ERROR: filed to get slock ticks per sec\n");
		exit(EXIT_FAILURE);
	}

	double dotprod_elapsed = (dotprod_end_etime - dotprod_start_etime) / (double) clktck;
	double elapsed = dotprod_elapsed;
	printf("%d,%d,%d,%d,%.3f,%.3f,%.3f,%.3f\n",n_rows_A,n_columns_A,n_columns_B,0,0.0,dotprod_elapsed,0.0,elapsed);	

	free(a);
	free(b);
	free(c);
	exit(EXIT_SUCCESS);
}
