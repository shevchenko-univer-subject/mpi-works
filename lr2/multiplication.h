#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
printMatrix(int **matrix, int rows, int cols);

void
multiplyMatrices(int **firstMatrix, int firstRows, int firstCols, 
		int **secondMatrix, int secondRows, int secondCols, 
		int ***resultMatrix, int *resultRows, int *resultCols);

void
initMatrix(int ***matrix, int *rows, 
		int *cols, const char *filename);
void
allocateMatrix(int ***matrix_p, int rows, int cols);
