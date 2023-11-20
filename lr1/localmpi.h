#include "mpi.h"
#include "employee_filtering.h"
#include <stdlib.h>


void mainPrintEmployees(int wrank, Employee *list, int size);
void calculatePartitionIndices(int wrank, int wsize, int lsize,
		int *start, int *end, int *chunk_size);
void mainInitializeEmployees(int wrank, char *filename, 
		Employee **list, int *size);
void bcastSize(int *size_p);
void initializeSizeAndRank(int *wsize, int *wrank);
void mainSendChunks(int wrank, int wsize, Employee *list, int list_size);
void otherProcessesRecvChunk(int wrank, int wsize, Employee *list,
		  	int size, Employee **sublist, int *chunk_size_p);
void summirizeFilteredSize(int chunk_size, int *total_size);
void collectFilteredLists(int wrank, int wsize, Employee** total_flist, 
		int total_fsize, Employee* chunk_flist, int chunk_fsize);
