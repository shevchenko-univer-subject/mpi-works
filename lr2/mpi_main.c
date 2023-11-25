#include "mpi.h"
#include "mpe.h"
#include "localmpi.h"

int
main()
{
	MPI_Init(&argc, &argv);

	int wrank, wsize;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
	MPI_Comm_size(MPI_COMM_WORLD, &wsize);

	int **a, **b, **c; // matrices
	int a_rows, a_cols;
	int b_rows, b_cols;
	int c_rows, c_cols;

	if (wrank == 0) {
		initMatrix(&a, &a_rows, &a_cols, "matrices/a.csv");
		initMatrix(&b, &b_rows, &b_cols, "matrices/b.csv");
	}

	c_rows = a_rows;
	c_cols = b_cols;

//////////// implementing
	int *l_a, *l_b, *l_c;
	// Receive parts of A and B
	mainSendChunks(world_rank, world_size, , total_size);
	// Allocate memory for localC
	matrixMultiplication(localA, localB, localC, rows / size, cols);

	if (wrank == 0) {
		// Gather results from all processes
		// Combine them into final matrix C
	}

	MPI_Finalize();
	return 0;
}

