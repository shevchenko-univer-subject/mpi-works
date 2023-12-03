#include "multiplication.h"
#include "mpi.h"
/**
 * Broadcasts the size of the matrix to all processes in the MPI communicator.
 *
 */
void bcastSize(int *size_p);

/**
 * Calculates the start and end indices for a matrix chunk 
 * that a specific process will handle. 
 * It also calculates the chunk size for each process based 
 * on its rank and total number of processes.
 */
void calculatePartitionIndices(int wrank, int wsize, int lsize, 
		int *start, int *end, int *chunk_size);

/**
 * Receives chunks of the first and second matrices on all processes other than the root.
 * It also initializes the chunk matrices and their dimensions.
 */
void otherProcessesRecvChunk(int wrank, int wsize, int size, int **first, int **second, 
                             int ***first_chunk_p, int *first_rows_p, int *first_cols_p, 
                             int ***second_chunk_p, int *second_rows_p, int *second_cols_p);

/**
 * Converts a linear array to a 2D matrix.
 */
void makeMatrixFromLinear(int ***matrix_p, int *linear, int rows, int cols);

/**
 * Sends chunks of the first and second matrices from the main 
 * process to other processes.
 */
void mainSendChunks(int wrank, int wsize, int **first, int **second, int size);

/**
 * Initializes a linear chunk of the second matrix.
 */
void initLinearChunkSecond(int **matrix_p, int chunk_size, 
		int size, int **whole, int start);

/**
 * Initializes a linear chunk of the first matrix.
 */
void initLinearChunkFirst(int **matrix_p, int chunk_size, int size, 
		int **whole, int start);

/**
 * Frees the memory allocated for multiple matrices and linear arrays.
 */
void freeMemory(int wrank, 
                int **a, int a_rows, 
                int **b, int b_rows, 
                int **c, int c_rows, 
                int **la, int la_rows, 
                int **lb, int lb_rows, 
                int **lc, int lc_rows);

