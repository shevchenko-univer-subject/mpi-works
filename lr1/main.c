#include "employee_filtering.h"
#include "localmpi.h"
#include "mpi.h"


int main()
{
	MPI_Init(NULL, NULL);
	initializeCurrentYear();

	int world_size, world_rank;
	int total_size, chunk_size, chunk_fsize, total_fsize;
	Employee *total_list = NULL, 
		 *chunk_list = NULL,
		 *chunk_flist = NULL, 
		 *total_flist = NULL;

	initializeSizeAndRank(&world_size, &world_rank);

	mainInitializeEmployees(world_rank, "employees.csv", 
			&total_list, &total_size);
	bcastSize(&total_size);

	mainSendChunks(world_rank, world_size, total_list, total_size);

	MPI_Barrier(MPI_COMM_WORLD);
	otherProcessesRecvChunk(world_rank, world_size, total_list,
			   	total_size, &chunk_list, &chunk_size);

	MPI_Barrier(MPI_COMM_WORLD);
	filterEmployees(chunk_list, chunk_size, &chunk_flist, 
			&chunk_fsize);

	MPI_Barrier(MPI_COMM_WORLD);
	summirizeFilteredSize(chunk_fsize, &total_fsize);
	
	MPI_Barrier(MPI_COMM_WORLD);
	collectFilteredLists(world_rank, world_size, &total_flist,
			   	total_fsize, chunk_flist, chunk_fsize);

	mainPrintEmployees(world_rank, total_flist, total_fsize);

	if (world_rank == 0)
	{
		free(total_list);
		free(total_flist);
	}
	free(chunk_list);
	free(chunk_flist);

	free(currentYearPtr);

	MPI_Finalize();
	return 0;
}
