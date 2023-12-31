#include "localmpi.h"

int FIRST_TAG = 100;
int SECOND_TAG = 200;
int OFFSET_TAG = 10;
int MATRIX_TAG = 1;
int CHUNK_SIZE_TAG = 2; 

void calculatePartitionIndices(int wrank, int wsize, int lsize,
                int *start, int *end, int *chunk_size)
{
        int base_chunk_size = lsize / wsize;
        int remainder = lsize % wsize;

        if (wrank < remainder) {
                *start = wrank * (base_chunk_size + 1);
                *end = *start + base_chunk_size;
        } else {
                *start = wrank * base_chunk_size + remainder;
                *end = *start + base_chunk_size - 1;
        }

        *chunk_size = *end - *start + 1;
}

void freeMemory(int wrank, 
		int **a, int a_rows,
		int **b, int b_rows,
		int **c, int c_rows,
		int **la, int la_rows,
		int **lb, int lb_rows,
		int **lc, int lc_rows)
{
	if(wrank == 0)
	{
		for (int i = 0; i < a_rows; i++)
			free(a[i]);
		free(a);

		for (int i = 0; i < b_rows; i++)
			free(b[i]);
		free(b);

		for (int i = 0; i < c_rows; i++)
			free(c[i]);
		free(c);

		for (int i = 0; i < la_rows; i++)
			free(la[i]);
		free(la);

		for (int i = 0; i < lb_rows; i++)
			free(lb[i]);
		free(lb);

		for (int i = 0; i < lc_rows; i++)
			free(lc[i]);
		free(lc);
	}
}


void
initLinearChunkFirst( int **matrix_p, int *rows_p, int *cols_p, int chunk_size, int size, 
		int **whole, int start)
{
	int rows = chunk_size;
	int cols = size;
	*matrix_p = (int *)malloc(rows * cols * sizeof(int));

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			(*matrix_p)[i*cols + j] = whole[start + i][j];
	*rows_p = rows;
	*cols_p = cols;
}


void
initLinearChunkSecond(int **matrix_p, int *rows_p, int *cols_p, int chunk_size, int size, 
		int **whole, int start)
{
	int rows = size;
	int cols = chunk_size; 
	*matrix_p = (int *)malloc(rows * cols * sizeof(int));

	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			(*matrix_p)[i*cols + j] = whole[i][start + j];

	*rows_p = rows;
	*cols_p = cols;
}


void 
sendChunk(int recv_rank, int send_rank, int parent_matrix, int size, void *initChunk)
{
	int start, end, chunk_size;
	calculatePartitionIndices(recv_rank, send_rank, size,
			&start, &end, &chunk_size);
	int *chunk;
	int rows, cols;
	*initChunk(&chunk, &rows, &cols, chunk_size, size, 
			parent_matrix, start);

	MPI_Send(rows, 1, MPI_INT, recv_rank, 
			ROWS_TAG, MPI_COMM_WORLD);

	MPI_Send(cols, 1, MPI_INT, recv_rank, 
			COLS_TAG, MPI_COMM_WORLD);

	MPI_Send(start, 1, MPI_INT, recv_rank,
			OFFSET_TAG, MPI_COMM_WORLD);

	MPI_Send(chunk, chunk_size * size,
			MPI_INT, i, MATRIX_TAG, MPI_COMM_WORLD);

	free(chunk_first);
}	


void 
mainSendChunks(int wrank, int wsize,
		int **first, int **second, int size)
{
	if (wrank == 0)
	{
		for(int i = 1; i < wsize; i++)
		{
			sendChunk(i, wrank, first, size, &initLinearChunkFirst);
			sendChunk(i, wrank, second, size, &initLinearChunkSecond);
		}

	}
}


void
makeMatrixFromLinear(int ***matrix_p, int *linear,
			int rows, int cols)
{
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			(*matrix_p)[i][j] = linear[i * cols + j];
}

/* 
 * NOTES: procedure in progress
 * BLOCKERS: unclear what i should to pass via mpi and why
 * 		hard to get logic of cols passing between processes
 * 			it should be another one procedure for passing
 * 		using of pointers on procedure. i've already written it 
 * 			in sendChunk, but i'm not sure is it correct
 *
 * TODO: 
 * 	- find example of dinamic procedure call
 * 	- write procedure for sending and cathing  cols between processes
 * 	- rewrite recvChunk procedure
 * 	
 */
void
recvChunk(int *first_rows_p, int firs)
			int ***first_chunk_p, int *first_rows_p, int *first_cols_p,
			int ***second_chunk_p, int *second_rows_p, int *second_cols_p,
{

	*first_rows_p = chunk_size;
	*first_cols_p = size;
	*second_rows_p = size;
	*second_cols_p = chunk_size;

	int first_rows = *first_rows_p;
	int first_cols = *first_cols_p;
	int second_rows = *second_rows_p;
	int second_cols = *second_cols_p;


	allocateMatrix(first_chunk_p, *first_rows_p, *first_cols_p);
	allocateMatrix(second_chunk_p, *second_rows_p, *second_cols_p);

	int *first_linear = (int *)malloc(first_rows * first_cols * sizeof(int));
	int *second_linear = (int *)malloc(second_rows * second_cols * sizeof(int));
}

void
otherProcessesRecvChunk(int wrank, int wsize, int size, int **first, int **second,
			int ***first_chunk_p, int *first_rows_p, int *first_cols_p,
			int ***second_chunk_p, int *second_rows_p, int *second_cols_p,
			int *offset)
{
	int start, end, chunk_size;
	calculatePartitionIndices(wrank, wsize, size,
		&start, &end, &chunk_size);

	*first_rows_p = chunk_size;
	*first_cols_p = size;
	*second_rows_p = size;
	*second_cols_p = chunk_size;

	int first_rows = *first_rows_p;
	int first_cols = *first_cols_p;
	int second_rows = *second_rows_p;
	int second_cols = *second_cols_p;


	allocateMatrix(first_chunk_p, *first_rows_p, *first_cols_p);
	allocateMatrix(second_chunk_p, *second_rows_p, *second_cols_p);

	int *first_linear = (int *)malloc(first_rows * first_cols * sizeof(int));
	int *second_linear = (int *)malloc(second_rows * second_cols * sizeof(int));

	if (wrank != 0)
	{
		MPI_Recv(first_linear, first_rows * first_cols,
				MPI_INT, 0, FIRST_TAG, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		makeMatrixFromLinear(first_chunk_p, first_linear,
				first_rows, first_cols);

		MPI_Recv(second_linear, second_rows * second_cols,
				MPI_INT, 0, SECOND_TAG, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
		makeMatrixFromLinear(second_chunk_p, second_linear,
				second_rows, second_cols);
		MPI_Recv(offset, 1, MPI_INT, 0, OFFSET_TAG, MPI_COMM_WORLD);
	} else {
		for(int i = 0; i < first_rows; i++)
			for(int j = 0; j < first_cols; j++)
				(*first_chunk_p)[i][j] = first[i + start][j];

		for(int i = 0; i < second_rows; i++)
			for(int j = 0; j < second_cols; j++)
				(*second_chunk_p)[i][j] = second[i][j + start];
		*offset = 0;
	}	

	free(first_linear);
	free(second_linear);
}

void
bcastSize(int *size_p)
{
        MPI_Bcast(size_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

