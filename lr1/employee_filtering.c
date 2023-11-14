#include "employee_filtering.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>


int *currentYearPtr = NULL;


int validateBirth(int birth)
{
	return birth >= 1900 && birth <= 2010;
}


int validateExp(int exp)
{
	int currentYear = *currentYearPtr;
	return exp >= 1950 && exp <= currentYear;
}


Employee parseLine(char *line)
{
	Employee emp;
	sscanf(line, "%49[^,],%49[^,],%d,%d", emp.fname, emp.lname, &emp.birth, &emp.exp_y);

	if (!validateBirth(emp.birth) || !validateExp(emp.exp_y)) {
		fprintf(stderr, "Invalid data found. Exiting.\n");
		exit(EXIT_FAILURE);
	}

	return emp;
}


int countLines(FILE *file)
{
	int lines = 0;
	char ch;
	while(!feof(file))
	{
		ch = fgetc(file);
		if (ch == '\n') {
			lines ++;
		}
	}
	rewind(file);
	return lines;

}


void validateFile(FILE *file)
{
	if (!file)
	{
		perror("Unable to open the file");
		exit(EXIT_FAILURE);
	}
}


Employee* readCSV(const char *filename, int size)
{
	FILE *file = fopen(filename, "r");

	Employee *employees = malloc(sizeof(Employee) * size);
	char line[200];
	int i = 0;

	while (fgets(line, sizeof(line), file))
	{
		employees[i++] = parseLine(line);
	}

	return employees;
}


void initializeCurrentYear()
{
    if (currentYearPtr == NULL)
    {
        currentYearPtr = (int *)malloc(sizeof(int));
        time_t now = time(NULL);
        struct tm *tm_now = localtime(&now);
        *currentYearPtr = tm_now->tm_year + 1900;
    }
}
