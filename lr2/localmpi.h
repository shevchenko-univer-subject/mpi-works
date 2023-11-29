#include "multiplication.h"
#include "mpi.h"

void bcastSize(int *size_p);

void
calculatePartitionIndices(int wrank, int wsize, int lsize,
                int *start, int *end, int *chunk_size);

void
otherProcessesRecvChunk(int wrank, int wsize, int size, int **first, int **second,
			int ***first_chunk_p, int *first_rows_p, int *first_cols_p,
			int ***second_chunk_p, int *second_rows_p, int *second_cols_p);
void
makeMatrixFromLinear(int ***matrix_p, int *linear,
			int rows, int cols);
void 
mainSendChunks(int wrank, int wsize,
		int **first, int **second, int size);
void
initLinearChunkSecond(int **matrix_p, int chunk_size, int size, 
		int **whole, int start);
void
initLinearChunkFirst( int **matrix_p, int chunk_size, int size, 
		int **whole, int start);

void freeMemory(int wrank, 
		int **a, int a_rows,
		int **b, int b_rows,
		int **c, int c_rows,
		int **la, int la_rows,
		int **lb, int lb_rows,
		int **lc, int lc_rows);
