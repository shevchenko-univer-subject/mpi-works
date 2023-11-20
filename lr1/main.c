#include <mpi.h>
#include "mpe.h"
#include "employee_filtering.h"
#include "localmpi.h"


int main()
{
	int world_size, world_rank;
	int total_size, chunk_size, chunk_fsize, total_fsize;
	Employee *total_list = NULL, 
		 *chunk_list = NULL,
		 *chunk_flist = NULL, 
		 *total_flist = NULL;
	int ev1a, ev1b, ev2a, ev2b, ev3a, ev3b, ev4a, ev4b;

	MPI_Init(NULL, NULL);
	initializeCurrentYear();
	initializeSizeAndRank(&world_size, &world_rank);

	MPE_Init_log();
	ev1a = MPE_Log_get_event_number();
        ev1b = MPE_Log_get_event_number();
        ev2a = MPE_Log_get_event_number();
        ev2b = MPE_Log_get_event_number();
        ev3a = MPE_Log_get_event_number();
        ev3b = MPE_Log_get_event_number();
        ev4a = MPE_Log_get_event_number();
        ev4b = MPE_Log_get_event_number();
	if(world_rank == 0)
	{
                MPE_Describe_state(ev1a, ev1b, "Broadcast", "red");
                MPE_Describe_state(ev2a, ev2b, "Compute", "blue");
                MPE_Describe_state(ev3a, ev3b, "Reduce", "green");
                MPE_Describe_state(ev4a, ev4b, "Sync", "orange");
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPE_Log_event(ev1a, 0, "start broadcast");
	mainInitializeEmployees(world_rank, "employees.csv", 
			&total_list, &total_size);
	bcastSize(&total_size);

	mainSendChunks(world_rank, world_size, total_list, total_size);

	otherProcessesRecvChunk(world_rank, world_size, total_list,
			   	total_size, &chunk_list, &chunk_size);
	MPE_Log_event(ev1b, 0, "end broadcast");

	MPE_Log_event(ev4a, 0, "start sync");
	MPI_Barrier(MPI_COMM_WORLD);
	MPE_Log_event(ev4b, 0, "end sync");

	MPE_Log_event(ev2a, 0, "start compute");
	filterEmployees(chunk_list, chunk_size, &chunk_flist, 
			&chunk_fsize);
	MPE_Log_event(ev2b, 0, "end compute");

	MPE_Log_event(ev4a, 0, "start sync");
	MPI_Barrier(MPI_COMM_WORLD);
	MPE_Log_event(ev4b, 0, "end sync");

	
	MPE_Log_event(ev3a, 0, "start reduce");
	summirizeFilteredSize(chunk_fsize, &total_fsize);
	collectFilteredLists(world_rank, world_size, &total_flist,
			   	total_fsize, chunk_flist, chunk_fsize);
	MPE_Log_event(ev3b, 0, "end reduce");

	mainPrintEmployees(world_rank, total_flist, total_fsize);

	if (world_rank == 0)
	{
		free(total_list);
		free(total_flist);
	}
	free(chunk_list);
	free(chunk_flist);

	free(currentYearPtr);

	MPE_Finish_log("./logs/graphlog");
	MPI_Finalize();
	return 0;
}
