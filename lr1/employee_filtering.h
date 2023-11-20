#ifndef EMPLOYEE_FILTERING_H
#define EMPLOYEE_FILTERING_H

#include <stdio.h>  // For FILE
#include <string.h>

// Define the Employee structure
typedef struct {
    char fname[50];
    char lname[50];
    int birth;
    int exp_y;
} Employee;

extern int *currentYearPtr;

// Function Declarations

/**
 * Validates the birth year of an employee.
 * @param birth Year of birth to validate.
 * @return 1 if valid, 0 otherwise.
 */
int validateBirth(int birth);

/**
 * Validates the experience year of an employee.
 * @param exp Year of experience to validate.
 * @return 1 if valid, 0 otherwise.
 */
int validateExp(int exp);

/**
 * Parses a line from the CSV file and creates an employee object.
 * @param line The line to parse.
 * @return An Employee object.
 */
Employee parseLine(char *line);

/**
 * Validate if file exist
 * @param FILE pointer
 * @return exit from program if file doesn't exist
 */
void validateFile(FILE *file);

/**
 * Reads the CSV file and returns an array of Employee objects.
 * @param filename Name of the CSV file.
 * @param size Pointer to hold the size of the returned array.
 * @return Pointer to the first element of the array of Employee objects.
 */
Employee* readCSV(const char *filename, int size);

/**
 * Initializes the current year and stores it in a global pointer.
 * Should be called once at the beginning of the program.
 */
void initializeCurrentYear();

int countLines(FILE *file);

void filterEmployees(Employee *list, int size, Employee **flist_p, int *fsize_p);

void initializeEmployees(char *filename, Employee **employees, int *size_p);

void printEmployees(Employee *list, int size);

#endif // EMPLOYEE_FILTERING_H

