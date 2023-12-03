#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
	float position;
	float velocity;
} participle;

void simulate_motion(participle *particles, int num_particles, float time_interval) {
	for (int i = 0; i < num_particles; i++) {
		particles[i].position += particles[i].velocity * time_interval;
	}
}

int main(int argc, char *argv[]) {
	int num_particles = 100000;
	clock_t start, end;
	double duration;

	participle *particles = malloc(num_particles * sizeof(participle));
	if (particles == NULL) {
		printf("Не вдалося виділити пам'ять.\n");
		return 1;
	}

	for (int i = 0; i < num_particles; i++) {
		particles[i].position = rand();
		particles[i].velocity = rand();
	}

	float time_interval = 10.0;
	start = clock();
	simulate_motion(particles, num_particles, time_interval);
	end = clock();
	duration = ((double) (end - start)) / CLOCKS_PER_SEC;

	printf("Calculation time: %lf\n", duration);

	free(particles);

	return 0;
}

