#include "localmpi.h"


void mainPrintEmployees(int wrank, Employee *list, int size)
{
	if (wrank == 0)
	{
		printEmployees(list, size);
	}
}


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


void mainPrintStatistic(int wrank, int total_size, int total_fsize)
{

	if(wrank == 0)
	{
		printf("Input:  %d records\n", total_size);
		printf("Output: %d records\n", total_fsize);
	}
}


void mainInitializeEmployees(int wrank, char *filename, 
		Employee **list, int *size)
{
	if (wrank == 0)
	{
		initializeEmployees("employees.csv", list, size);
	}
}


void bcastSize(int *size_p)
{
	MPI_Bcast(size_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
}


void initializeSizeAndRank(int *wsize, int *wrank)
{
	MPI_Comm_size(MPI_COMM_WORLD, wsize);
	MPI_Comm_rank(MPI_COMM_WORLD, wrank);
}


void mainSendChunks(int wrank, int wsize, Employee *list, int list_size)
{
	if (wrank == 0)
	{
		for(int i = 1; i < wsize; i++)
		{
			int start, end, chunk_size;
			calculatePartitionIndices(i, wsize, list_size,
					&start, &end, &chunk_size);

			MPI_Send(&list[start], chunk_size * sizeof(Employee), 
					MPI_BYTE, i, 0, MPI_COMM_WORLD);
		}
	}
}


void otherProcessesRecvChunk(int wrank, int wsize, Employee *list,
		  	int size, Employee **sublist, int *chunk_size_p)
{
		int start, end, chunk_size;
		calculatePartitionIndices(wrank, wsize, size, 
			&start, &end, &chunk_size);
		*chunk_size_p = chunk_size;
	*sublist = (Employee *)malloc(sizeof(Employee) * chunk_size);

	if (wrank != 0) 
	{
		MPI_Recv(*sublist, chunk_size * sizeof(Employee), 
				MPI_BYTE, 0, 0, MPI_COMM_WORLD,
				MPI_STATUS_IGNORE);
	} else {
		for(int i = start; i < chunk_size; i++)
		{
			(*sublist)[i] = list[i];
			fflush(stdout);
		}
	}
}


void summirizeFilteredSize(int chunk_size, int *total_size)
{
	MPI_Reduce(&chunk_size, total_size, 1, MPI_INT, 
			MPI_SUM, 0, MPI_COMM_WORLD);	
}


void collectFilteredLists(int wrank, int wsize, Employee** total_flist, 
		int total_fsize, Employee* chunk_flist, int chunk_fsize) 
{
	if (wrank == 0) 
	{
		*total_flist = (Employee *)malloc(sizeof(Employee) * total_fsize);

		memcpy(*total_flist, chunk_flist, chunk_fsize * sizeof(Employee));

		int offset = chunk_fsize;
		MPI_Status status;

		for (int i = 1; i < wsize; i++) {
			int size;
			MPI_Recv(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);

			MPI_Recv(*total_flist + offset, size * sizeof(Employee), 
					MPI_BYTE, i, 1, MPI_COMM_WORLD, &status);
			offset += size;
		}
	} else {
		MPI_Send(&chunk_fsize, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(chunk_flist, chunk_fsize * sizeof(Employee), 
				MPI_BYTE, 0, 1, MPI_COMM_WORLD);
	}
}

