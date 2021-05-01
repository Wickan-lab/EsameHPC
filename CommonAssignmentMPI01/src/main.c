#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "utils.h"

#define FILE_A "matrix_a"
#define FILE_B "matrix_b"
#define FILE_RES "matrix_res"
/* 
 * ===  FUNCTION  ======================================================================
 *  Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] ){
	// Get process rank and size
	int rank, size;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);

	// Parse parameters
	if(argc != 5){
		if(rank == 0) 
			fprintf(stderr,"Usage:\n\t%s [n_rows_A] [n_columns_A] [n_rows_B] [n_columns_B]\n",\
					argv[0]);
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}
	int n_rows_A    = atoi(argv[1]);
	int n_columns_A = atoi(argv[2]);
	int n_rows_B    = atoi(argv[3]);
	int n_columns_B = atoi(argv[4]);

	// Check matrix dimensions
	if(n_columns_A != n_rows_B){
		if(rank == 0) 
			fprintf(stderr,"Error: n_columns_A != n_rows_B\n");
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}

	// Open two files, one for each matrix
	MPI_File fh_a,fh_b;
	MPI_File_open(MPI_COMM_WORLD, FILE_A, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh_a);
	MPI_File_open(MPI_COMM_WORLD, FILE_B, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh_b);

	// Row datatype for A matrix
	MPI_Datatype dt_row_a;
	MPI_Type_contiguous(n_columns_A,MPI_DOUBLE,&dt_row_a);
	MPI_Type_commit(&dt_row_a);

#if VERSION == 5
	// Column datatype for B matrix
	MPI_Datatype dt_column_b;
	MPI_Type_vector(n_columns_B, 1, n_rows_B, MPI_DOUBLE, &dt_column_b);
	MPI_Type_commit(&dt_column_b);
#endif

	MPI_Barrier(MPI_COMM_WORLD);

	// --------------- MATRICES READ ---------------

	double read_start_time = MPI_Wtime();
	// Calculate chunk size for each process

	int chunk_size_A_rows = n_rows_A / size;
	if(rank == (size-1)){
		chunk_size_A_rows = n_rows_A - rank * chunk_size_A_rows ; 
	}

	double *a = (double*) malloc(sizeof(double) * chunk_size_A_rows * n_columns_A);

#if VERSION == 6
	double *b = (double*) malloc(sizeof(double) * n_rows_B);
#else
	double *b = (double*) malloc(sizeof(double) * n_rows_B * n_columns_B);
#endif

	// Read matrices in a contiguous fashion
	// Multiple versions of reading matrix A
#if VERSION == 1
	MPI_File_read_ordered(fh_a,a,chunk_size_A_rows ,dt_row_a,MPI_STATUS_IGNORE);

#elif VERSION == 2
	int displacement = rank * (n_rows_A / size) * n_columns_A * sizeof(double);
	MPI_File_seek(fh_a, displacement, MPI_SEEK_SET);
	MPI_File_read_all(fh_a, a, chunk_size_A_rows, dt_row_a, MPI_STATUS_IGNORE);

#elif VERSION == 3
	int displacement = rank * (n_rows_A / size) * n_columns_A * sizeof(double);
	MPI_File_set_view(fh_a, displacement, MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh_a, a, chunk_size_A_rows * n_columns_A, MPI_DOUBLE, MPI_STATUS_IGNORE);

#elif VERSION == 4
	int displacement = rank * (n_rows_A / size) * n_columns_A * sizeof(double);
	MPI_File_set_view(fh_a, displacement, MPI_BYTE, dt_row_a, "native", MPI_INFO_NULL);
	MPI_File_read(fh_a, a, chunk_size_A_rows, dt_row_a, MPI_STATUS_IGNORE);

#elif VERSION == 5
	int displacement_row = rank * (n_rows_A / size) * n_columns_A * sizeof(double);
	MPI_File_set_view(fh_a, displacement_row, MPI_BYTE, dt_row_a, "native", MPI_INFO_NULL);
	MPI_File_read(fh_a, a, chunk_size_A_rows, dt_row_a, MPI_STATUS_IGNORE);
	for (int i = 0; i < n_rows_B; ++i)
		MPI_File_read(fh_b, b + i, 1, dt_column_b, MPI_STATUS_IGNORE);
#elif VERSION == 6
	MPI_File_read_ordered(fh_a,a,chunk_size_A_rows ,dt_row_a,MPI_STATUS_IGNORE);
	MPI_Datatype dt_transpose_b;
	MPI_Type_contiguous(n_rows_B, MPI_DOUBLE, &dt_transpose_b);
	MPI_Type_commit(&dt_transpose_b);

#endif


#if VERSION != 5 && VERSION != 6
	MPI_File_read(fh_b, b, n_rows_B*n_columns_B, MPI_DOUBLE, MPI_STATUS_IGNORE);
#endif

	double read_end_time = MPI_Wtime();
	// ----------------------------------------

	// --------------- DOT PROD ---------------

	

#if VERSION == 6
	
	double *c = (double*) malloc(sizeof(double) * chunk_size_A_rows);
	MPI_File fh_c;
	MPI_File_delete(FILE_RES, MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, FILE_RES, MPI_MODE_CREATE | MPI_MODE_RDWR,MPI_INFO_NULL, &fh_c);

	double dot_prod_start_time = MPI_Wtime();
	for (int ncol = 0; ncol < n_columns_B; ncol++){

		//matrix B in file is read by rows, since it transposed
		MPI_File_read(fh_b, b, 1, dt_transpose_b, MPI_STATUS_IGNORE);

		matrix_dot_matrix(a, b, c, chunk_size_A_rows, n_columns_A, n_rows_B, 1);

		for (int y = 0; y < chunk_size_A_rows; y++){
			//c[chunk_size_A_rows * rank + y][ncol]
			MPI_File_seek(fh_c, (((n_rows_A/size) * rank + y) * n_columns_B + ncol)*sizeof(double), MPI_SEEK_SET);
			MPI_File_write(fh_c, c + y, 1,MPI_DOUBLE,MPI_STATUS_IGNORE);
			//(chunk_size_A_rows * rank + y) * n_columns_B + ncol
			//(((n_rows_A/size) * rank + y) * n_columns_B + ncol)*sizeof(double) perche' mi serve l'inizio di dove shcrivere la matrice, l'ultimo
			//ha chunk + grande
		}
	}

	MPI_Type_free(&dt_transpose_b);
#else
	double *c = (double*) malloc(sizeof(double) * chunk_size_A_rows * n_columns_B);

	double dot_prod_start_time = MPI_Wtime();	
	matrix_dot_matrix(a, b, c, chunk_size_A_rows, n_columns_A, n_rows_B, n_columns_B);
	double dot_prod_end_time = MPI_Wtime();

	double write_start_time = MPI_Wtime();
	MPI_File fh_c;
	MPI_File_delete(FILE_RES, MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, FILE_RES, MPI_MODE_CREATE | \
			MPI_MODE_RDWR,MPI_INFO_NULL, &fh_c);
	MPI_File_write_ordered(fh_c, c, chunk_size_A_rows * n_columns_B, \
			MPI_DOUBLE,MPI_STATUS_IGNORE);
	double write_end_time = MPI_Wtime();

#endif

	// ----------------------------------------
	
	double read_time = read_end_time - read_start_time;
	double dot_prod_time = dot_prod_end_time - dot_prod_start_time;

#if VERSION == 6
	double write_time = 0;
#else
	double write_time = write_end_time - write_start_time;
#endif

	double global_read_time, global_dot_prod_time, global_write_time;
	MPI_Reduce(&read_time,&global_read_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Reduce(&dot_prod_time,&global_dot_prod_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	MPI_Reduce(&write_time,&global_write_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

	double global_elapsed = global_read_time + global_dot_prod_time + global_write_time;
	if(rank == 0) printf("%d,%d,%d,%d,%.3f,%.3f,%.3f,%.3f\n",n_rows_A, n_columns_A, n_columns_B, size, global_read_time, global_dot_prod_time, global_write_time, global_elapsed);	

	free(c);
	free(a);
	free(b);

#ifdef DEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_set_view(fh_c, 0, MPI_BYTE, MPI_BYTE, "native", MPI_INFO_NULL);
	if(rank == 0){
		c = (double*) malloc(sizeof(double) * n_rows_A * n_columns_B);
		MPI_File_read(fh_c, c, n_rows_A * n_columns_B, MPI_DOUBLE, MPI_STATUS_IGNORE);
		for(int i = 0; i < n_rows_A; i++){
			for(int j = 0; j < n_columns_B; j++){
				printf("%.2f ",c[i*n_columns_B + j]);
			}
			putchar('\n');
		}
		free(c);
	}
#endif

	MPI_File_close(&fh_c);
	MPI_File_close(&fh_a);
	MPI_File_close(&fh_b);
	MPI_Type_free(&dt_row_a);

#if VERSION == 5
	MPI_Type_free(&dt_column_b);
#endif
	MPI_Finalize();

	exit(EXIT_SUCCESS);
}
