#define _CRT_SECURE_NO_WARNINGS
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/ //uncomment this block to check for heap memory allocation leaks.
// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct StudentCourseGrade
{
	char courseName[35];
	int grade;
}StudentCourseGrade;

typedef struct Student
{
	char name[35];
	StudentCourseGrade* grades; //dynamic array of courses
	int numberOfCourses;
}Student;


//Part A
void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
int countPipes(const char* lineBuffer, int maxCount);
char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents);
void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents);
void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor);
void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int coursesPerStudent[9] = { 0 };
	int numberOfStudents = 0;
	countStudentsAndCourses("studentList.txt", &coursesPerStudent, &numberOfStudents);
	char*** students = makeStudentArrayFromFile("studentList.txt", &coursesPerStudent, &numberOfStudents);
	factorGivenCourse(students, coursesPerStudent, numberOfStudents, "Advanced Topics in C", +5);
	printStudentArray(students, coursesPerStudent, numberOfStudents);
	//studentsToFile(students, coursesPerStudent, numberOfStudents); //this frees all memory. Part B fails if this line runs. uncomment for testing (and comment out Part B)
	
	//Part B
	Student* transformedStudents = transformStudentArray(students, coursesPerStudent, numberOfStudents);
	writeToBinFile("students.bin", transformedStudents, numberOfStudents);
	Student* testReadStudents = readFromBinFile("students.bin");

	//add code to free all arrays of struct Student


	/*_CrtDumpMemoryLeaks();*/ //uncomment this block to check for heap memory allocation leaks.
	// Read https://docs.microsoft.com/en-us/visualstudio/debugger/finding-memory-leaks-using-the-crt-library?view=vs-2019

	return 0;
}

void countStudentsAndCourses(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	FILE* fin =	fopen(fileName, "r");
	int student_cnt = 1;
	int** courses = coursesPerStudent;
	rewind(fin);
	while (feof(fin) == 0)
	{
		if (getc(fin) == '\n')
			student_cnt++;
	}
	*numberOfStudents = student_cnt;

	rewind(fin);

	int i = 0;
	while (feof(fin) == 0)
	{
		char str[1023];
		int res = countPipes (fgets(str, 1023, fin),1023);
		*coursesPerStudent = res;
		coursesPerStudent++;
	}

	fclose(fin);
}

int countPipes(const char* lineBuffer, int maxCount)
{
	if (lineBuffer == NULL)
		return -1;
	if (maxCount <= 0)
		return 0;
	int counter = 0;
	for (int i = 0; i < maxCount && *lineBuffer != '\0'; i++)
	{
		if (*lineBuffer == '|')
			counter++;

		lineBuffer++;
	}
	return counter;
}

char*** makeStudentArrayFromFile(const char* fileName, int** coursesPerStudent, int* numberOfStudents)
{
	//add code here
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	if (factor < 20 || factor > -20)
		return;
}

void printStudentArray(const char* const* const* students, const int* coursesPerStudent, int numberOfStudents)
{
	for (int i = 0; i < numberOfStudents; i++)
	{
		printf("name: %s\n*********\n", students[i][0]);
		for (int j = 1; j <= 2 * coursesPerStudent[i]; j += 2)
		{
			printf("course: %s\n", students[i][j]);
			printf("grade: %s\n", students[i][j + 1]);
		}
		printf("\n");
	}
}

void studentsToFile(char*** students, int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	//add code here
}

Student* readFromBinFile(const char* fileName)
{
	//add code here
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	//add code here
}
