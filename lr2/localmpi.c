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

