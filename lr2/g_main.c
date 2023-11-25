#include "multiplication.h"
#include <time.h>

int
main()
{
	int **a, **b, **c; // matrices
	int a_rows, a_cols;
	int b_rows, b_cols;
	int c_rows, c_cols;

	clock_t start, end;
	double duration;

	initMatrix(&a, &a_rows, &a_cols, "matrices/a.csv");
	initMatrix(&b, &b_rows, &b_cols, "matrices/b.csv");

	start = clock();
	multiplyMatrices(a, a_rows, a_cols,
			 b, b_rows, b_cols, 
			&c, &c_rows, &c_cols);
	end = clock();
	duration = ((double) (end - start)) / CLOCKS_PER_SEC;

	printMatrix(c, c_rows, c_cols);
	printf("Calculation time: %lf\n", duration);

	for (int i = 0; i < a_rows; i++)
		free(a[i]);

	for (int i = 0; i < b_rows; i++)
		free(b[i]);

	for (int i = 0; i < c_rows; i++)
		free(c[i]);

	free(a);
	free(b);
	free(c);

	return 0;
}
