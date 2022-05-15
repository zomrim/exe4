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
int* countRowTavs(char* buffer, int arr_size);

//Part B
Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents);
void writeToBinFile(const char* fileName, Student* students, int numberOfStudents);
Student* readFromBinFile(const char* fileName);

int main()
{
	//Part A
	int* coursesPerStudent = NULL;
	int numberOfStudents = 0;
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
	FILE* fin = fopen(fileName, "r");
	if (!fin) printf("Unable to open file!");
	int student_cnt = 1;
	rewind(fin);
	while (feof(fin) == 0)
	{
		if (getc(fin) == '\n')
			student_cnt++;
	}
	*numberOfStudents = student_cnt;

	int* courses = (int*)malloc(sizeof(int) * student_cnt);
	if (!courses) { exit(1); }
	int** coursesPtr = courses;
	rewind(fin);
	int i = 0;
	while (feof(fin) == 0)
	{
		char str[1023];
		int res = countPipes(fgets(str, 1023, fin), 1023);
		*coursesPtr = res;
		coursesPtr++;
	}
	*coursesPerStudent = courses;
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
	FILE* fin = fopen(fileName, "r");
	if (!fin) printf("Unable to open file!");
	countStudentsAndCourses(fileName, coursesPerStudent, numberOfStudents);
	int* coursesPtr = *coursesPerStudent;
	char*** students = (char***)malloc(sizeof(char**) * (*numberOfStudents));
	if (!students) { exit(1); }
	int*** studentsPtr = students;
	for (int i = 0; i < (*numberOfStudents); i++)
	{
		int iter = (*coursesPtr) * 2 + 1;
		char** student = (char**)malloc(sizeof(char*) * iter);
		if (!student) { exit(1); }
		char** studentPtr = student;
		*studentsPtr = student;
		char line[1023];
		fgets(line, 1023, fin);
		int* sizes = countRowTavs(line, iter);
		int* sizesPtr = sizes;
		char* name = (char*)malloc(sizeof(char) * (*sizesPtr));
		if (!name) { exit(1); }
		*studentPtr = name;
		studentPtr++;
		char* buffer = strtok(line, "|,\n");
		strcpy(name, buffer);
		buffer = strtok(NULL, "|,\n");
		sizesPtr++;
		for (int j = 0; j < (*coursesPtr) * 2; j++)
		{
			char* data = (char*)malloc(sizeof(char) * (*sizesPtr));
			if (!data) { exit(1); }
			*studentPtr = data;
			strcpy(data, buffer);
			buffer = strtok(NULL, "|,\n");
			studentPtr++;
			sizesPtr++;
		}
		coursesPtr++;
		studentsPtr++;
	}
	return students;
	fclose(fin);
}

void factorGivenCourse(char** const* students, const int* coursesPerStudent, int numberOfStudents, const char* courseName, int factor)
{
	if (factor > 20 || factor < -20)
		return;

	int* coursesperStudentPtr = coursesPerStudent;
	for (int i = 0; i < numberOfStudents; i++)
	{
		char** dataPtr = *students;
		for (int j = 0; j < (*coursesPerStudent) * 2 + 1; j++)
		{
			if (strcmp(*dataPtr, courseName) == 0)
			{
				dataPtr++;
				int grade = atoi(*dataPtr);
				grade += factor;
				if (grade >= 100)
					grade = 100;
				if (grade <= 0)
					grade = 0;
				_itoa(grade, *dataPtr, 10);
			}
			else dataPtr++;
		}
		coursesPerStudent++;
		students++;
	}
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
	FILE* fin = fopen("studentList.txt", "w");
	if (!fin) printf("Unable to open file!");
	char*** studentsPtr = students;
	int* courses = coursesPerStudent;
	for (int i = 0; i < numberOfStudents; i++)
	{
		char** studentData = *studentsPtr;
		fputs(*studentData, fin);
		free(*studentData);

		for (int j = 0; j < *courses; j++)
		{
			studentData++;
			fputc('|', fin);
			fputs(*studentData, fin);
			free(*studentData);
			studentData++;
			fputc(',', fin);
			fputs(*studentData, fin);
			free(*studentData);
		}
		free(*studentsPtr);

		studentsPtr++;
		courses++;

		fputc('\n', fin);
	}
	fclose(fin);
}

void writeToBinFile(const char* fileName, Student* students, int numberOfStudents)
{
	FILE* fin = fopen(fileName, "wb");
	if (!fin) printf("Unable to open file!");
	fwrite(&numberOfStudents, sizeof(int), 1, fin);
	Student* studentsPtr = students;
	for (int i = 0; i < numberOfStudents; i++)
	{
		StudentCourseGrade* coursesPtr = studentsPtr->grades;
		fwrite(studentsPtr->name, sizeof(char), 35, fin);
		fwrite(&(studentsPtr->numberOfCourses), sizeof(int), 1, fin);
		for (int j = 0; j < studentsPtr->numberOfCourses; j++)
		{
			fwrite(coursesPtr->courseName, sizeof(char), 35, fin);
			fwrite(&(coursesPtr->grade), sizeof(int), 1, fin);
			coursesPtr++;
		}
		studentsPtr++;
	}
	fclose(fin);
}

Student* readFromBinFile(const char* fileName)
{
	FILE* fin = fopen(fileName, "rb");
	if (!fin) printf("Unable to open file!");
	int numOfStudent; 
	fread(&numOfStudent, sizeof(int), 1, fin);
	Student* students = (Student*)malloc(sizeof(Student) * numOfStudent);
	if (!students) { exit(1); }
	Student* studentsPtr = students;
	for (int i = 0; i < numOfStudent; i++)
	{
		Student s; 
		fread((s.name), sizeof(char), 35, fin);
		fread(&(s.numberOfCourses), sizeof(int), 1, fin);
		StudentCourseGrade* courses = (StudentCourseGrade*)malloc(sizeof(StudentCourseGrade) * s.numberOfCourses);
		if (!courses) { exit(1); }
		StudentCourseGrade* coursesPtr = courses;
		for (int j = 0; j < s.numberOfCourses; j++)
		{
			fread((coursesPtr->courseName), sizeof(char), 35, fin);
			fread(&(coursesPtr->grade), sizeof(int), 1, fin);
			coursesPtr++;
		}
		s.grades = courses;
		*studentsPtr = s;
		studentsPtr++;
	}
	fclose(fin);
	return students;
}

Student* transformStudentArray(char*** students, const int* coursesPerStudent, int numberOfStudents)
{
	Student* students_arr = (Student*)malloc(sizeof(Student) * numberOfStudents);
	if (!students_arr) { exit(1); }
	Student* arr_ptr = students_arr;
	char*** studentsPtr = students;
	int* coursesNum = coursesPerStudent;
	for (int i = 0; i < numberOfStudents; i++)
	{
		char** studentdata = *studentsPtr;
		Student s;
		strcpy(s.name, *studentdata);
		studentdata++;
		s.numberOfCourses = *coursesNum;
		StudentCourseGrade* courses = (StudentCourseGrade*)malloc(sizeof(StudentCourseGrade) * (*coursesNum));
		if (!courses) { exit(1); }
		StudentCourseGrade* coursesPtr = courses;
		for (int j = 0; j < *coursesNum; j++)
		{
			StudentCourseGrade course;
			strcpy(course.courseName, *studentdata);
			studentdata++;
			course.grade = atoi(*studentdata);
			studentdata++;
			*coursesPtr = course;
			coursesPtr++;
		}
		s.grades = courses;
		*arr_ptr = s;
		arr_ptr++;
		coursesNum++;
		studentsPtr++;
	}
	return students_arr;
}

int* countRowTavs(char* buffer, int arr_size)
{
	int* sizes = (int*)malloc(sizeof(int) * arr_size);
	int* sizesPtr = sizes;
	int counter = 0;
	char* ptrline = buffer;
	int i = 0;
	while (*ptrline != '\0')
	{
		while (*ptrline != '|' && *ptrline != ',' && *ptrline != '\0')
		{
			if (*ptrline == '\n')
				break;
			counter++;
			ptrline++;
		}
		*sizesPtr = counter + 1;
		ptrline++;
		sizesPtr++;
		counter = 0;
	}
	return sizes;
}

