#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define N 496 // Matrix size N x N
#define NUM_THREADS 16 // Number of threads

void initMatrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            matrix[i][j] = rand() % 10;
}

void printMatrix(double matrix[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%6.2f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void matrixMultiply(double a[N][N], double b[N][N], double c[N][N]) {
    int blockSize = N / (NUM_THREADS / 2); // Assuming NUM_THREADS is a perfect square

    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int thread_id = omp_get_thread_num();
        int row_block = thread_id / (NUM_THREADS / 2);
        int col_block = thread_id % (NUM_THREADS / 2);

        for (int k = 0; k < (NUM_THREADS / 2); ++k) {
            for (int i = row_block * blockSize; i < (row_block + 1) * blockSize; ++i) {
                for (int j = col_block * blockSize; j < (col_block + 1) * blockSize; ++j) {
                    double sum = 0.0;
                    for (int l = k * blockSize; l < (k + 1) * blockSize; ++l) {
                        sum += a[i][l] * b[l][j];
                    }
                    #pragma omp critical
                    c[i][j] += sum;
                }
            }
        }
    }
}

int main() {
    double a[N][N], b[N][N], c[N][N] = {0};
    time_t start, end;
    double diff;

    initMatrix(a);
    initMatrix(b);

    start = clock();
    matrixMultiply(a, b, c);
    end = clock();

    diff = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("%d %d %lf\n",NUM_THREADS, N, diff);
    return 0;
}

