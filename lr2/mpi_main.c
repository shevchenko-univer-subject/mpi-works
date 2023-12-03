#include "mpi.h"
#include "mpe.h"
#include "localmpi.h"


void
set_neigborhoods(int rank, int size, int *prev_p, int *next_p)
{
	int prev, next;
	prev = rank - 1;
	next = next + 1;

	if(prev = -1)
		prev = size - 1;
	if(next = size)
		next = 0
	
	*prev_p = prev;
	*next_p = next;
}

void
set_process_info(int *rank_p, int *size_p, int *prev_p, int *next_p)
{
	MPI_Comm_rank(MPI_COMM_WORLD, rank_p);
	MPI_Comm_size(MPI_COMM_WORLD, size_p);
	set_neigborhoods(*rank_p, *size_p, prev_p, *next_p)
}

void
copy_chunk_to_local_result_by_offset(int **chunk, int ***row_matrix, int col_offset,
	       	int cols, int rows)
{
	for(int i = 0; i < rows; i++)
		for(int j = 0; j < cols; j++)
			(*row_matrix)[i][j + col_offset] = chunk[i][j];
}

int
main()
{
	MPI_Init(NULL, NULL);

	int wrank, wsize, wprev, wnext;
	set_process_info(&wrank, &wsize, &wprev, &wnext);

	// main matrices
	int **a, **b, **c;
	int a_rows, a_cols;
	int b_rows, b_cols;
	int c_rows, c_cols;
	int size;

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
	mainSendChunksAndOffsets(wrank, wsize, a, b, size);

	MPI_Barrier(MPI_COMM_WORLD);

	// local matrices (chunks)
	int **la, **lb, **lc, **lc_row;
	int la_rows, la_cols;
	int lb_rows, lb_cols;
	int lc_rows, lc_cols;
	int lchunk_rows, lchunk_cols;
	int offset, rowset, colset;

	otherProcessesRecvChunkAndOffset(wrank, wsize, size, a, b,
			&la, &la_rows, &la_cols,
			&lb, &lb_rows, &lb_cols, &offset);


	rowset = offset;
	colset = offset;

	// allocate row
	allocateMatrix(&lc, la_rows, size);

	MPI_Barrier(MPI_COMM_WORLD);

	do
	{
		/*
		 *  TODO: do-while replece with for(int i = 0; i < wsize; i++)
		 *  NOTES: 
		 *  	i don't need separate column and row offset 
		 *
		 *  here chunks is already initialized
		 *  then they're multipliyed 
		 *  then the result will be placed in lc matrix by the offset
		 *  then the second chunk (lb) will be sent to next process
		 *  then the second chunk will be recieved from previoud process
		 *  and ones more
		 */
		multiplyMatrices(la, la_rows, la_cols,
				 lb, lb_rows, lb_cols,
				 &lchunk, &lchunk_rows, &lchunk_cols);

		// lchunk_rows should be eq to la_rows
		copy_chunk_to_local_result_by_offset(lchunk, &lc, colset, 
				lchunk_cols, lchunk_rows);

		if(wrank % 2 == 0)
		{
			MPI_Send(
		} else {
		}
	}
	while(1);
	/*
	 * each process send its lc to the main one
	 */



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

