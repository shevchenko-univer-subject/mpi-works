#include "employee_filtering.h"
#include <stdlib.h>

int main()
{
	initializeCurrentYear();
	int currentYear = *currentYearPtr;

	const char *filename = "employees.csv";
	FILE *file = fopen(filename, "r");

	validateFile(file);
	int size = countLines(file);
	Employee *employees = readCSV(filename, size);

	fclose(file);

	printf("Filtered Employees:\n");
	for (int i = 0; i < size; i++)
	{
		if (currentYear - employees[i].exp_y > 5)
		{
			printf("%s %s\n", employees[i].fname, employees[i].lname);
		}
	}

	free(employees);
	free(currentYearPtr);
	return 0;
}
