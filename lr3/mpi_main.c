#include <mpi.h>
#include "mpe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int MPI_CHUNK_SIZE_TAG = 11;
int MPI_CHUNK_TAG = 12;
int MPI_START_TAG = 13;


typedef struct {
	float position;
	float velocity;
} participle;

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

void simulate_motion(participle *particles, int num_particles, float time_interval) {
	for (int i = 0; i < num_particles; i++) {
		particles[i].position += particles[i].velocity * time_interval;
	}
}

int main(int argc, char *argv[])
{
	// INTIALIZATION
	MPI_Init(&argc, &argv);
	MPE_Init_log();
	int ev1a, ev1b, ev2a, ev2b, ev3a, ev3b, ev4a, ev4b;
	ev1a = MPE_Log_get_event_number();
	ev1b = MPE_Log_get_event_number();
	ev2a = MPE_Log_get_event_number();
	ev2b = MPE_Log_get_event_number();
	ev3a = MPE_Log_get_event_number();
	ev3b = MPE_Log_get_event_number();
	ev4a = MPE_Log_get_event_number();
	ev4b = MPE_Log_get_event_number();

	int wrank, wsize;
	MPI_Comm_rank(MPI_COMM_WORLD, &wrank);
	MPI_Comm_size(MPI_COMM_WORLD, &wsize);
	if(wrank == 0)
	{
		MPE_Describe_state(ev1a, ev1b, "Broadcast", "red");
		MPE_Describe_state(ev2a, ev2b, "Compute", "blue");
		MPE_Describe_state(ev3a, ev3b, "Reduce", "green");
		MPE_Describe_state(ev4a, ev4b, "Sync", "orange");
	}

	const int nitems = 2;
	int blocklengths[2] = {1, 1};
	MPI_Datatype types[2] = {MPI_FLOAT, MPI_FLOAT};
	MPI_Datatype mpi_participle_type;
	MPI_Aint offsets[2];

	offsets[0] = offsetof(participle, position);
	offsets[1] = offsetof(participle, velocity);
	MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_participle_type);
	MPI_Type_commit(&mpi_participle_type);

	int start, end, chunk_size;
	participle *chunk, *particles;

	if (wrank == 0)
	{

		int num_particles = 1000;

		particles = malloc(num_particles * sizeof(participle));
		if (particles == NULL)
		{
			printf("Не вдалося виділити пам'ять.\n");
			return 1;
		}

		for (int i = 0; i < num_particles; i++)
		{
			particles[i].position = (float)rand();
			particles[i].velocity = (float)rand(); 
		}
		calculatePartitionIndices(wrank, wsize, num_particles,
				&start, &end, &chunk_size);
		chunk = malloc(chunk_size * sizeof(participle));
		memcpy(chunk, particles + start, chunk_size * sizeof(participle));

		for (int i = 1; i < wsize; i++)
		{
			int start_tmp, end_tmp, chunk_size_tmp;
			calculatePartitionIndices(i, wsize, num_particles,
					&start_tmp, &end_tmp, &chunk_size_tmp);
			printf("will send on %d\n", i);

			MPI_Send(&chunk_size_tmp, 1, MPI_INT, i, MPI_CHUNK_SIZE_TAG, MPI_COMM_WORLD);
			MPI_Send(&start_tmp, 1, MPI_INT, i, MPI_START_TAG, MPI_COMM_WORLD);
			MPI_Send(particles + start_tmp, chunk_size_tmp, 
					mpi_participle_type, i, MPI_CHUNK_TAG, MPI_COMM_WORLD);
		}
	} else {
		printf("will recv on %d\n", wrank);
		MPI_Recv(&chunk_size, 1, MPI_INT, 0, MPI_CHUNK_SIZE_TAG, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv(&start, 1, MPI_INT, 0, MPI_START_TAG, 
				MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		chunk = malloc(chunk_size * sizeof(participle));
		MPI_Recv(chunk, chunk_size, 
				mpi_participle_type, 0, MPI_CHUNK_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}

	float time_interval = 10.0;
	simulate_motion(chunk, chunk_size, time_interval);

	if(wrank != 0){
		printf("2: will send on %d\n", wrank);
		MPI_Send(&start, 1, MPI_INT, 0, MPI_START_TAG, MPI_COMM_WORLD);
		MPI_Send(&chunk_size, 1, MPI_INT, 0, MPI_CHUNK_SIZE_TAG, MPI_COMM_WORLD);
		MPI_Send(chunk, chunk_size, 
				mpi_participle_type, 0, MPI_CHUNK_TAG, MPI_COMM_WORLD);
	} else {
		memcpy(particles, chunk, chunk_size * sizeof(participle));
		for(int i = 1; i < wsize; i++) 
		{
			printf("2: will recv on %d\n", i);
			int start_tmp, chunk_size_tmp;
			MPI_Recv(&start_tmp, 1, MPI_INT, i, MPI_START_TAG, 
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&chunk_size_tmp, 1, MPI_INT, i, MPI_CHUNK_SIZE_TAG, 
					MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(particles + start_tmp, chunk_size_tmp, 
					mpi_participle_type, i, MPI_CHUNK_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}

	free(chunk);
	if(wrank == 0)
		free(particles);
	MPI_Type_free(&mpi_participle_type);
	MPI_Finalize();
	return 0;
}

