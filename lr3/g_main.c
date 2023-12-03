#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float position;  // Координата
    float velocity;  // Швидкість
} participle;

// Функція для оновлення позицій частинок
void simulate_motion(participle *particles, int num_particles, float time_interval) {
    for (int i = 0; i < num_particles; i++) {
        // Оновлення позиції кожної частинки
        particles[i].position += particles[i].velocity * time_interval;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Використання: %s <кількість частинок>\n", argv[0]);
        return 1;
    }

    int num_particles = atoi(argv[1]);
    if (num_particles <= 0) {
        printf("Введіть додатнє число для кількості частинок.\n");
        return 1;
    }

    participle *particles = malloc(num_particles * sizeof(participle));
    if (particles == NULL) {
        printf("Не вдалося виділити пам'ять.\n");
        return 1;
    }

    // Ініціалізація частинок
    for (int i = 0; i < num_particles; i++) {
        particles[i].position = rand();
        particles[i].velocity = rand(); // Припускаємо однакову швидкість для всіх частинок
    }

    // Симуляція руху
    float time_interval = 10.0;
    simulate_motion(particles, num_particles, time_interval);

    // Вивід результатів
    for (int i = 0; i < num_particles; i++) {
        printf("Частинка %d: Позиція = %f\n", i+1, particles[i].position);
    }

    free(particles);

    return 0;
}

