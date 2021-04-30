#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

#define FILE_A "matrix_a"
#define FILE_B "matrix_b"

int main(int argc, char**argv){
	
	if(argc != 6){
		fprintf(stderr,"Usage:\n\t%s [n_rows_A] [n_columns_A] [n_rows_B] [n_columns_B] [VERSION]\n",argv[0]);
		exit(EXIT_FAILURE);
	}	
	
	int n_rows_A    = atoi(argv[1]);
	int n_columns_A = atoi(argv[2]);
	int n_rows_B    = atoi(argv[3]);
	int n_columns_B = atoi(argv[4]);
	int version = atoi(argv[5]);
	double *a,*b;

	init(&a,&b,n_rows_A,n_columns_A,n_rows_B,n_columns_B);

	//TVB LT
	FILE*file_a;
	FILE*file_b;

	file_b = fopen(FILE_B,"w");
	file_a = fopen(FILE_A,"w");

	fwrite(a,sizeof(double),n_rows_A*n_columns_A,file_a);

	if (version == 5){
		fwrite(b,sizeof(double),n_rows_B*n_columns_B,file_b);

	}else{

		for (int i = 0; i < n_columns_B; i++){
			for(int j = 0; j < n_rows_B; j++){
				fwrite(b + j*n_columns_B + i,sizeof(double),1,file_b);
			}
		//Il numero dell'universo line:42	
		}		
	}

	fclose(file_b);
	fclose(file_a);
	exit(EXIT_SUCCESS);
}
