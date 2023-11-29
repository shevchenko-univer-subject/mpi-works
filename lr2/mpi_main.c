#include "mpi.h"
#include "mpe.h"
#include "localmpi.h"

void
initLinearChunkFirst( int **matrix_p, int chunk_size, int size, 
		int **whole, int start);
{
	int rows = chunk_size;
	int cols = size;
	*matrix_p = (int *)malloc(rows * cols * sizeof(int));

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			(*matrix_p)[i*cols + j] = whole[start + i][j];
}


void
initLinearChunkSecond(int **matrix_p, int chunk_size, int size, 
		int **whole, int start);
{
	int rows = size;
	int cols = chunk_size; 
	*matrix_p = (int *)malloc(rows * cols * sizeof(int));

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			(*matrix_p)[i*cols + j] = whole[i][start + j];
}


void 
mainSendChunks(int wrank, int wsize,
		int **a, int **b, int size)
{
	if (wrank == 0)
	{
		for(int i = 1; i < wsize; i++)
		{
			int start, end, chunk_size;
			calculatePartitionIndices(i, wsize, size,
					&start, &end, &chunk_size);

			int *chunk_first;
			int *chunk_second;

			initLinearChunkFirst(&chunk_first, chunk_size, size, 
					a, start);
			initLinearChunkSecond(&chunk_second, size, chunk_size, 
					b, start);

			int *chunk_first_b =  &(chunk_first[0][0])
			int *chunk_second_b = &(chunk_second[0][0])

			MPI_Send(chunk_first_b, chunk_size * size,
					MPI_INT, i, 0, MPI_COMM_WORLD);
			MPI_Send(chunk_second_b, chunk_size * size,
					MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}

	free(chunk_first);
	free(chunk_second);
}


void
makeMatrixFromLinear(int ***matrix_p, int *linear,
			int rows, int cols)
{
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			*(matrix)[i][j] = linear[i * cols + j];
}


void
otherProcessesRecvChunk(int wrank, int wsize, int size,
			int ***first_chunk_p, int *first_rows_p, int *first_cols_p,
			int ***second_chunk_p, int *second_rows_p, int *second_cols_p)
{
	int start, end, chunk_size;
	calculatePartitionIndices(wrank, wsize, size,
		&start, &end, &chunk_size);

	*first_rows_p = size;
	*first_cols_p = chunk_size;
	*second_rows_p = chunk_size;
	*second_cols_p = size;

	int first_rows = *first_rows_p
	int first_cols = *first_cols_p
	int second_rows = *second_rows_p
	int second_cols = *second_cols_p


	allocateMatrix(first_chunk_p, *first_rows_p, *first_cols_p);
	allocateMatrix(second_chunk_p, *second_rows_p, *second_cols_p);

	int *first_linear = (int *)malloc(first_rows * first_cols * sizeof(int));
	int *second_linear = (int *)malloc(second_rows * second_cols * sizeof(int));


	if (wrank != 0)
	{
		MPI_Recv(first_linear, first_rows * first_rows,
				MPI_INT, 0, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		makeMatrixFromLinear(&first_chunk_p, first_linear,
				first_rows, first_cols);

		MPI_Recv(second_linear, second_rows * second_rows,
				MPI_INT, 0, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		makeMatrixFromLinear(&second_chunk_p, second_linear,
				second_rows, second_cols);
	} 

	free(first_linear);
	free(second_linear);
}

void 
mainProcessRecvChunk(int ***first_chunk, int ***second_chunk,
		int **first, int **second, int size)
{
	int start, end, chunk_size;
	calculatePartitionIndices(wrank, wsize, size,
		&start, &end, &chunk_size);

	for(int i = start; i < end; i++)
		for(int j = 0; j < size; j++)
			*(first_chunk)[i][j] = first[i][j];

	for(int i = 0; i < size; i++)
		for(int j = start; j < end; j++)
			*(second_chunk)[i][j] = second[i][j];
}


int
main()
{
	MPI_Init(&argc, &argv);

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
	int *la, *lb, *lc;
	int la_rows, la_cols;
	int lb_rows, lb_cols;
	int lc_rows, lc_cols;


	if (wrank == 0) 
	{
		initMatrix(&a, &a_rows, &a_cols, "matrices/a.csv");
		initMatrix(&b, &b_rows, &b_cols, "matrices/b.csv");

		c_rows = a_rows;
		c_cols = b_cols;
		allocateMatrix(&c, c_rows, c_cols);
	}

	if(a_rows != a_cols && b_rows != b_cols && a_rows != b_rows){
		perror("Error. Matrices form isn't square");
		return;
	} 

	size = a_rows; // or other, not important

//////////// implementing
	// Receive parts of A and B
	mainSendChunks(world_rank, world_size, , total_size);
	// Allocate memory for localC
	matrixMultiplication(localA, localB, localC, rows / size, cols);

	if (wrank == 0) {
		// Gather results from all processes
		// Combine them into final matrix C
	}

	MPI_Finalize();

	freeMemory(wrank, 
			a, a_rows
			b, b_rows,
		       	c, c_rows,
			la, la_rows, 
			lb, lb_rows, 
			lc, lc_rows);

	return 0;
}

