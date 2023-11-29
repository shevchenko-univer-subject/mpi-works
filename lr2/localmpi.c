#include "localmpi.h"

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
		int **lc, int lc_rows,
	       )
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
