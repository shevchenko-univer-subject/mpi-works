#include "mpi.h"
#include "mpe.h"
#include "localmpi.h"


int
main()
{
	MPI_Init(NULL, NULL);

	int wrank, wsize;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
	MPI_Comm_size(MPI_COMM_WORLD, &wsize);

	// main matrices
	int **a, **b, **c;
	int a_rows, a_cols;
	int b_rows, b_cols;
	int c_rows, c_cols;
	int size;

	// local matrices (chunks)
	int **la, **lb, **lc;
	int la_rows, la_cols;
	int lb_rows, lb_cols;
	int lc_rows, lc_cols;

	if (wrank == 0) 
	{
		initMatrix(&a, &a_rows, &a_cols, "matrices/a.csv");
		initMatrix(&b, &b_rows, &b_cols, "matrices/b.csv");
		if(a_rows != a_cols && b_rows != b_cols && a_rows != b_rows){
			perror("Error. Matrices form isn't square");
			return 1;
		} 

		c_rows = a_rows;
		c_cols = b_cols;
		allocateMatrix(&c, c_rows, c_cols);

		size = a_rows; // or other, not important
	}
	
	bcastSize(&size);

	mainSendChunks(wrank, wsize, a, b, size);

	MPI_Barrier(MPI_COMM_WORLD);

	otherProcessesRecvChunk(wrank, wsize, size, a, b,
			&la, &la_rows, &la_cols,
			&lb, &lb_rows, &lb_cols);
	MPI_Barrier(MPI_COMM_WORLD);

	multiplyMatrices(la, la_rows, la_cols,
			 lb, lb_rows, lb_cols,
			 &lc, &lc_rows, &lc_cols);

	printf("a: rank: %d, rows : %d,cols %d\n", wrank, la_rows, la_cols);
	printf("b: rank: %d, rows : %d,cols %d\n", wrank, lb_rows, lb_cols);
	printf("c: rank: %d, rows : %d,cols %d\n", wrank, lc_rows, lc_cols);


	if (wrank == 0) {
		printMatrix(lc, lc_rows, lc_cols);
	}

	MPI_Finalize();

	freeMemory(wrank, 
			a, a_rows,
			b, b_rows,
		       	c, c_rows,
			la, la_rows, 
			lb, lb_rows, 
			lc, lc_rows);

	return 0;
}

