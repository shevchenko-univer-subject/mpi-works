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
/**
 * Counts the number of lines in a given file.
 * 
 * @param file A pointer to a FILE object from which to count lines.
 * @return The number of lines in the file.
 */
int countLines(FILE *file);

/**
 * Filters the given list of employees based on a specific criteria and returns a new list.
 * 
 * @param list The original list of employees to be filtered.
 * @param size The number of employees in the original list.
 * @param flist_p Pointer to store the filtered list of employees.
 * @param fsize_p Pointer to store the number of employees in the filtered list.
 */
void filterEmployees(Employee *list, int size, Employee **flist_p, int *fsize_p);

/**
 * Reads employee data from a file and initializes an array of Employee structures.
 * 
 * @param filename The name of the file to read employee data from.
 * @param employees Pointer to store the array of initialized Employee structures.
 * @param size_p Pointer to store the number of employees read from the file.
 */
void initializeEmployees(char *filename, Employee **employees, int *size_p);

/**
 * Prints the details of each employee in the given list.
 * 
 * @param list The list of employees to be printed.
 * @param size The number of employees in the list.
 */
void printEmployees(Employee *list, int size);

#endif // EMPLOYEE_FILTERING_H

