#include "employee_filtering.h"
#include <stdlib.h>
#include "mpi.h"


int main()
{
	initializeCurrentYear();

	MPI_Init(NULL, NULL);

	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	int size, fsize;
	Employee *list, *flist;
	if (world_rank == 0)
	{
		initializeEmployees("employees.csv", &list, &size);
	}
	filterEmployees(list, 0, size, &flist, &fsize);

	free(flist);
	free(list);
	free(currentYearPtr);

	MPI_Finalize();
	return 0;
}
