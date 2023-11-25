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
		int ***resultMatrix, int *resultRows, int *resultCols) {

	if (firstCols != secondRows)
	{
		printf("Matrix multiplication not possible.\n");
		return;
	}

	*resultMatrix = (int **)malloc(firstRows * sizeof(int *));
	*resultRows = firstRows;
	*resultCols = secondCols;

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

