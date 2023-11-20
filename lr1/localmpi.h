// libmpi.h

#include "mpi.h"
#include "employee_filtering.h"
#include <stdlib.h>

/**
 * Print the list of employees. This function should be called only by the main process.
 * @param wrank The rank of the process within MPI_COMM_WORLD.
 * @param list The list of employees to print.
 * @param size The number of employees in the list.
 */
void mainPrintEmployees(int wrank, Employee *list, int size);

/**
 * Calculate the start and end indices for partitioning a list among MPI processes.
 * @param wrank The rank of the process within MPI_COMM_WORLD.
 * @param wsize The total number of processes in MPI_COMM_WORLD.
 * @param lsize The total size of the list to be partitioned.
 * @param start Pointer to store the calculated start index.
 * @param end Pointer to store the calculated end index.
 * @param chunk_size Pointer to store the size of the chunk.
 */
void calculatePartitionIndices(int wrank, int wsize, int lsize, int *start, int *end, int *chunk_size);

/**
 * Initialize the list of employees in the main process.
 * @param wrank The rank of the process within MPI_COMM_WORLD.
 * @param filename The name of the file from which to load the employees.
 * @param list Pointer to store the allocated list of employees.
 * @param size Pointer to store the number of employees in the list.
 */
void mainInitializeEmployees(int wrank, char *filename, Employee **list, int *size);

/**
 * Broadcast the size of the list to all processes in MPI_COMM_WORLD.
 * @param size_p Pointer to the size variable to broadcast.
 */
void bcastSize(int *size_p);

/**
 * Initialize the size and rank variables for the MPI environment.
 * @param wsize Pointer to store the number of processes.
 * @param wrank Pointer to store the rank of the current process.
 */
void initializeSizeAndRank(int *wsize, int *wrank);

/**
 * Send chunks of the employee list from the main process to other processes.
 * @param wrank The rank of the process within MPI_COMM_WORLD.
 * @param wsize The total number of processes in MPI_COMM_WORLD.
 * @param list The list of employees to send.
 * @param list_size The size of the list.
 */
void mainSendChunks(int wrank, int wsize, Employee *list, int list_size);

/**
 * Receive a chunk of the employee list in processes other than the main process.
 * @param wrank The rank of the process within MPI_COMM_WORLD.
 * @param wsize The total number of processes in MPI_COMM_WORLD.
 * @param list The original list of employees (used by the main process).
 * @param size The size of the original list.
 * @param sublist Pointer to store the allocated chunk of the list.
 * @param chunk_size_p Pointer to store the size of the received chunk.
 */
void otherProcessesRecvChunk(int wrank, int wsize, Employee *list, int size, Employee **sublist, int *chunk_size_p);

/**
 * Summarize the size of the filtered list from each process.
 * @param chunk_size The size of the filtered chunk in the current process.
 * @param total_size Pointer to store the total size of all filtered chunks.
 */
void summirizeFilteredSize(int chunk_size, int *total_size);

/**
 * Collect the filtered lists from all processes in the main process.
 * @param wrank The rank of the process within MPI_COMM_WORLD.
 * @param wsize The total number of processes in MPI_COMM_WORLD.
 * @param total_flist Pointer to store the combined filtered list (used by the main process).
 * @param total_fsize The total size of the combined filtered list.
 * @param chunk_flist The filtered list in the current process.
 * @param chunk_fsize The size of the filtered list in the current process.
 */
void collectFilteredLists(int wrank, int wsize, Employee** total_flist, int total_fsize, Employee* chunk_flist, int chunk_fsize);

void mainPrintStatistic(int wrank, int total_size, int total_fsize);
