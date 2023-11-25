#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
initMatrix(int ***matrix, int *rows, 
		int *cols, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening file");
		return;
	}

	char line[1024];
	int rowCount = 0;
	int colCount = 0;

	while (fgets(line, 1024, file))
	{
		if (rowCount == 0)
		{
			char *token = strtok(line, ",");
			while (token)
			{
				colCount++;
				token = strtok(NULL, ",");
			}
		}
		rowCount++;
	}

	*matrix = (int **)malloc(rowCount * sizeof(int *));
	for (int i = 0; i < rowCount; i++)
	{
		(*matrix)[i] = (int *)malloc(colCount * sizeof(int));
	}

	rewind(file);

	int row = 0;
	while (fgets(line, 1024, file) && row < rowCount)
	{
		char *token = strtok(line, ",");
		int col = 0;
		while (token)
		{
			(*matrix)[row][col++] = atoi(token);
			token = strtok(NULL, ",");
		}
		row++;
	}

	fclose(file);

	*rows = rowCount;
	*cols = colCount;
}


void
multiplyMatrices(int **firstMatrix, int firstRows, int firstCols, 
		int **secondMatrix, int secondRows, int secondCols, 
		int ***resultMatrix) {

	if (firstCols != secondRows)
	{
		printf("Matrix multiplication not possible.\n");
		return;
	}

	*resultMatrix = (int **)malloc(firstRows * sizeof(int *));
	for (int i = 0; i < firstRows; i++)
	{
		(*resultMatrix)[i] = (int *)malloc(secondCols * sizeof(int));
	}

	for (int i = 0; i < firstRows; i++)
	{
		for (int j = 0; j < secondCols; j++)
			(*resultMatrix)[i][j] = 0;
	}

	for (int i = 0; i < firstRows; i++)
	{
		for (int j = 0; j < secondCols; j++)
		{
			for (int k = 0; k < firstCols; k++)
				(*resultMatrix)[i][j] += 
					firstMatrix[i][k] * secondMatrix[k][j];
		}
	}
}


void
printMatrix(int **matrix, int rows, int cols)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			printf("%d\t", matrix[i][j]);
		printf("\n"); 
	}
}


int
main() {
	int **a, **b, **c; // matrices
	int a_rows, a_cols;
	int b_rows, b_cols;
	int c_rows, c_cols;

	initMatrix(&a, &a_rows, &a_cols, "matrices/a.csv");
	initMatrix(&b, &b_rows, &b_cols, "matrices/b.csv");

	multiplyMatrices(a, a_rows, a_cols,
			b, b_rows, b_cols, 
			&c);
	c_rows = a_rows;
	c_cols = b_cols;

	printMatrix(c, c_rows, c_cols);

	for (int i = 0; i < a_rows; i++) {
		free(a[i]);
	}
	for (int i = 0; i < b_rows; i++) {
		free(b[i]);
	}
	for (int i = 0; i < c_rows; i++) {
		free(c[i]);
	}

	free(a);
	free(b);
	free(c);

	return 0;
}
