#include "multiplication.h"

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

	allocateMatrix(matrix, rowCount, colCount);

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
allocateMatrix(int ***matrix_p, int rows, int cols)
{
	*matrix_p = (int **)malloc(rows * sizeof(int *));
	for (int i = 0; i < rows; i++)
		(*matrix_p)[i] = (int *)malloc(cols * sizeof(int));
}


void
multiplyMatrices(int **firstMatrix, int firstRows, int firstCols, 
		int **secondMatrix, int secondRows, int secondCols, 
		int ***result_p, int *result_rows_p, int *result_cols_p)
{
	if (firstCols != secondRows)
	{
		printf("Matrix multiplication not possible.\n");
		return;
	}

	*result_p = (int **)malloc(firstRows * sizeof(int *));
	*result_rows_p = firstRows;
	*result_cols_p = secondCols;
	int result_rows = *result_rows_p;
	int result_cols = *result_cols_p;

	allocateMatrix(result_p, result_rows, result_cols);

	for (int i = 0; i < result_rows; i++)
		for (int j = 0; j < result_cols; j++)
			(*result_p)[i][j] = 0;

	for (int i = 0; i < result_rows; i++)
		for (int j = 0; j < result_cols; j++)
			for (int k = 0; k < firstCols; k++)
				(*result_p)[i][j] += 
					firstMatrix[i][k] * secondMatrix[k][j];
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

