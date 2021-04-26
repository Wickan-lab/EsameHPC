#include	<stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "utils.h"

#define FILE_A "matrix_a"
#define FILE_B "matrix_b"
#define FILE_RES "matrix_res"
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	MPI_Init(&argc,&argv);

	// Get process rank
	int rank = 0;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	// Parse parameters
	if(argc != 5){
		if(rank == 0) 
			fprintf(stderr,"Usage:\n\t%s [n_rows_A] [n_columns_A] [n_rows_B] [n_columns_B]\n",argv[0]);
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
	MPI_File_delete(FILE_A, MPI_INFO_NULL);
	MPI_File_delete(FILE_B, MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, FILE_A, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh_a);
	MPI_File_open(MPI_COMM_WORLD, FILE_B, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh_b);

	// Row datatype for A matrix
	MPI_Datatype dt_row_a;
	MPI_Type_contiguous(n_columns_A,MPI_DOUBLE,&dt_row_a);
	MPI_Type_commit(&dt_row_a);

	// Process with rank 0 initializes the matrixes
	if(rank == 0){
		double *a,*b;
		init(&a,&b,n_rows_A,n_columns_A,n_rows_B,n_columns_B);

		MPI_Datatype dt_column_b;
		MPI_Type_vector(n_rows_B,1,n_columns_B,MPI_DOUBLE,&dt_column_b);
		MPI_Type_commit(&dt_column_b);

		// And writes them on file
		MPI_File_write(fh_a, a, n_rows_A, dt_row_a,MPI_STATUS_IGNORE);
		for(int i = 0; i < n_columns_B; i++){
			MPI_File_write(fh_b,b+i,1,dt_column_b,MPI_STATUS_IGNORE);
		}
		
		MPI_Type_free(&dt_column_b);
		
		// Process with rank 0 will read the file
		// after
		MPI_File_seek(fh_a,0,MPI_SEEK_SET);
		MPI_File_seek(fh_b,0,MPI_SEEK_SET);

	}

	// Calculate chunk size for each process
	int size = 0;
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	int chunk_size_A_rows = n_rows_A / size;
	if(rank == (size-1)){
		chunk_size_A_rows = n_rows_A - rank * chunk_size_A_rows ; 
	}

	double *a = (double*) malloc(sizeof(double) * chunk_size_A_rows * n_columns_A);
	double *b = (double*) malloc(sizeof(double) * n_rows_B * n_columns_B);

	// Read matrices in a contiguous fashion
	//		Multiple versions of reading matrix A
#if VERSION == 1
	MPI_File_read_ordered(fh_a,a,chunk_size_A_rows ,dt_row_a,MPI_STATUS_IGNORE);
#elif VERSION == 2
	int displacement = rank * (n_rows_A / size) * n_columns_A * sizeof(double);
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_File_seek(fh_a, displacement, MPI_SEEK_SET);
	MPI_File_read_all(fh_a, a, chunk_size_A_rows, dt_row_a, MPI_STATUS_IGNORE);
#elif VERSION == 3
	int displacement = rank * (n_rows_A / size) * n_columns_A * sizeof(double);
	MPI_File_set_view(fh_a, displacement, MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);
	MPI_File_read_all(fh_a, a, chunk_size_A_rows, dt_row_a, MPI_STATUS_IGNORE);
#endif

	// Read matrix B
	MPI_Type_free(&dt_row_a);
	MPI_File_read(fh_b,b,n_rows_B*n_columns_B,MPI_DOUBLE,MPI_STATUS_IGNORE);

	double *c = (double*) malloc(sizeof(double) * chunk_size_A_rows * n_columns_B);
	matrix_dot_matrix(a,b,c,chunk_size_A_rows,n_columns_A,n_rows_B,n_columns_B);

	MPI_File fh_c;
	MPI_File_delete(FILE_RES, MPI_INFO_NULL);
	MPI_File_open(MPI_COMM_WORLD, FILE_RES, MPI_MODE_CREATE | MPI_MODE_RDWR,MPI_INFO_NULL, &fh_c);
	MPI_File_write_ordered(fh_c, c, chunk_size_A_rows * n_columns_B, MPI_DOUBLE,MPI_STATUS_IGNORE);

	free(c);
	free(a);
	free(b);

	MPI_Barrier(MPI_COMM_WORLD);
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

	MPI_File_close(&fh_c);
	MPI_File_close(&fh_a);
	MPI_File_close(&fh_b);
	MPI_Finalize();
	exit(EXIT_SUCCESS);
}				/* ----------  end of function main  ---------- */
