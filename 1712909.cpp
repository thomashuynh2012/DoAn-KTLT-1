#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#define MAX_LINES 2048

//STUDENT functions

struct STUDENT
{
	wchar_t no[11];
	wchar_t name[31];
	wchar_t faculty[31];
	wchar_t email[50];
	int year;
	wchar_t birthday[11];
	wchar_t image[100];
	wchar_t bio[1001];
	wchar_t **hobby;
};
STUDENT GetSTUDENT(FILE *fp, int &nHobby, int CSVformat, int EmailField);
void ShowSTUDENT(STUDENT student, int nHobby, int EmailField);

//FILE functions

int CountLine(FILE* fp);
int CountHobby(FILE *fp, int CSVformat);
void FileCopy(FILE *fin, FILE *fout);
bool IsInArray(int *a, int A, int number);
void HTMLEdit(FILE* fout, STUDENT student, int *choice, int nChoice, int nHobby);
void HTMLGenerate(FILE *HTMLSource, STUDENT student, int *choice, int nChoice, int nHobby);
int UserChoice(int *&choice, int &CSVformat, int &EmailField);

//main

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	// _______________OPEN STUDENTS INFO FILE_______________
	bool error; //Error flag
	FILE *StudentFile = NULL;
	wchar_t StudentFileName[50];
	do
	{
		wprintf(L"Students CSV file name (DEFAULT: \"students.csv\"): ");
		wscanf(L"%ls", StudentFileName);
		StudentFile = _wfopen(StudentFileName, L"r, ccs=UTF-8");
		if (StudentFile == NULL) {
			wprintf(L"\nCAN NOT OPEN FILE!\n\n");
			error = true;
		}
		else
			error = false;
	} while (error == true);

	// _______________OPEN HTML SOURCE FILE_______________
	wchar_t *HTMLSourceName = L"HTMLSource.html";
	FILE *HTMLSource = _wfopen(HTMLSourceName, L"r, ccs=UTF-8");
	if (HTMLSource == NULL)
	{
		wprintf(L"\nCAN NOT OPEN HTML SOURCE FILE. PLEASE CHECK AGAIN!");
		_getch();
		return;
	}

	// _______________USER'S INPUT_______________
	int CSVformat; //Choose the CSV format
	int EmailField; //Check if there is email field
	int *choice = NULL;
	int nChoice = UserChoice(choice, CSVformat, EmailField);

	// _______________READ STUDENT INFO, GENERATE HTML FILE_______________
	int nStudent = CountLine(StudentFile); //Number of students is number of lines
	int nHobby; //Number of hobbies of each student 
	STUDENT *student = (STUDENT*)malloc(nStudent * sizeof(STUDENT));

	for (int i = 0; i < nStudent; i++)
	{
		student[i] = GetSTUDENT(StudentFile, nHobby, CSVformat, EmailField);
		ShowSTUDENT(student[i], nHobby, EmailField);
		wprintf(L"\n\n");
		HTMLGenerate(HTMLSource, student[i], choice, nChoice, nHobby);
	}

	// _______________FINISH. CLOSE FILES AND DELETE ALLOCATED ARRAYS_______________
	wprintf(L"\nSUCCESSFUL!");
	wprintf(L"\n%d PROFILE PAGES GENERATED", nStudent);
	fclose(HTMLSource);
	fclose(StudentFile);
	free(student);
	free(choice);
	_getch();
}

STUDENT GetSTUDENT(FILE *fp, int &nHobby, int CSVformat, int EmailField) //Read student from CSV file
{
	STUDENT s;
	if (CSVformat == 0)
	{
		if (EmailField == 1) // If there is email field in CSV file
			fwscanf(fp, L" \"%[^\",]\",\"%[^\",]\",\"%[^\",]\",\"%[^\",]\",\"%d\",\"%[^\",]\",\"%[^\",]\",\"%[^\"]\"",
				s.no, s.name, s.faculty, s.email, &s.year, s.birthday, s.image, s.bio);
		else
			fwscanf(fp, L" \"%[^\",]\",\"%[^\",]\",\"%[^\",]\",\"%d\",\"%[^\",]\",\"%[^\",]\", \"%[^\"]\"",
				s.no, s.name, s.faculty, &s.year, s.birthday, s.image, s.bio);
	}
	else if (CSVformat == 1)
	{
		if (EmailField == 1)
			fwscanf(fp, L" %[^,],%[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^\n,]",
				s.no, s.name, s.faculty, s.email, &s.year, s.birthday, s.image, s.bio);
		else
			fwscanf(fp, L" %[^,],%[^,],%[^,],%d,%[^,],%[^,],%[^\n,]",
				s.no, s.name, s.faculty, &s.year, s.birthday, s.image, s.bio);
	}
	nHobby = CountHobby(fp, CSVformat);

	if (nHobby == 0) //If student has no hobby
		s.hobby = NULL;
	else //If student has hobby, read hobby
	{
		s.hobby = (wchar_t**)malloc(nHobby * sizeof(wchar_t*));
		for (int i = 0; i < nHobby; i++)
		{
			s.hobby[i] = (wchar_t*)malloc(100 * sizeof(wchar_t));
			if (CSVformat == 0)
				fwscanf(fp, L",\"%[^\"\n]\"", s.hobby[i]);
			else
				fwscanf(fp, L",%[^,\n]", s.hobby[i]);
		}
	}
	return s;
}

void ShowSTUDENT(STUDENT student, int nHobby, int EmailField)
{
	wprintf(L"Number: %s\n", student.no);
	wprintf(L"Name: %ls\n", student.name);
	wprintf(L"Faculty: %ls\n", student.faculty);
	if (EmailField == 1)
		wprintf(L"Email: %ls\n", student.email);
	wprintf(L"Academic year: %d\n", student.year);
	wprintf(L"Birthday: %s\n", student.birthday);
	wprintf(L"Image link: %s\n", student.image);
	wprintf(L"Bio: %ls\n", student.bio);
	for (int i = 0; i < nHobby; i++)
		wprintf(L"Hobby: %ls\n", student.hobby[i]);
}

int CountLine(FILE* fp) //Count number of lines of file
{
	int pos = ftell(fp);
	int count = 0; wchar_t line[MAX_LINES];
	while (fgetws(line, MAX_LINES - 1, fp) != NULL)
		count++;
	fseek(fp, pos, SEEK_SET);
	return count;
}

int CountHobby(FILE *fp, int CSVformat) //Count number of hobbies of each student
{
	int nHobby;
	int pos = ftell(fp);

	if (CSVformat == 0) //If csv file has type 0, number of hobbies is number of quotation marks devided by 2
	{
		int count = 0;
		while (1)
		{
			wchar_t ch = fgetwc(fp);
			if (ch == L'"')
				count++;
			else if (ch == L'\n' || ch == WEOF)
				break;
		}
		nHobby = count / 2;
	}
	else if (CSVformat == 1) //If csv file has type 1, number of hobbies is number of commas
	{
		int count = 0;
		while (1)
		{
			wchar_t ch = fgetwc(fp);
			if (ch == L',')
				count++;
			else if (ch == L'\n' || ch == WEOF)
				break;
		}
		nHobby = count;
	}

	fseek(fp, pos, SEEK_SET);
	return nHobby;
}

void FileCopy(FILE *fin, FILE *fout) //Copy whole file from fin to fout
{
	rewind(fin);
	rewind(fout);
	wchar_t ch;
	while (1)
	{
		ch = fgetwc(fin);
		if (!feof(fin))
			fputwc(ch, fout);
		else
			break;
	}
}

bool IsInArray(int *a, int A, int number) //Check if number is in array a or not
{
	for (int i = 0; i < A; i++)
		if (a[i] == number)
			return true;

	return false;
}

void HTMLEdit(FILE* fout, STUDENT student, int *choice, int nChoice, int nHobby)
{
	rewind(fout);
	int count_line = 1;
	wchar_t line[MAX_LINES];
	while (fgetws(line, MAX_LINES - 1, fout) != NULL)
	{
		count_line++;
		fseek(fout, ftell(fout), SEEK_SET);
		switch (count_line)
		{
		case 6:
			fwprintf(fout, L"\t\t<title>HCMUS - %ls</title>", student.name);
			break;
		case 23:
			if (IsInArray(choice, nChoice, 1)) //Check if 1 is in the user's choice array
				fwprintf(fout, L"\t\t\t\t\t\t<div class=\"Personal_FullName\">%ls - %ls</div>", student.name, student.no);
			break;
		case 24:
			if (IsInArray(choice, nChoice, 3)) 
				fwprintf(fout, L"\t\t\t\t\t\t<div class=\"Personal_Department\">Khoa %ls</div>", student.faculty);
			break;
		case 27:
			if (IsInArray(choice, nChoice, 4))
				fwprintf(fout, L"\t\t\t\t\t\t\tEmail: %ls", student.email);
			break;
		case 34:
			fwprintf(fout, L"\t\t\t\t\t\t<img src=\"%ls\" class=\"Personal_Hinhcanhan\" />", student.image);
			break;
		case 47:
			if (IsInArray(choice, nChoice, 1))
				fwprintf(fout, L"\t\t\t\t\t\t\t\t<li>Họ và tên: %ls</li>", student.name);
			break;
		case 48:
			if (IsInArray(choice, nChoice, 2))
				fwprintf(fout, L"\t\t\t\t\t\t\t\t<li>MSSV: %ls</li>", student.no);
			break;
		case 49:
			if (IsInArray(choice, nChoice, 3))
				fwprintf(fout, L"\t\t\t\t\t\t\t\t<li>Sinh viên khoa: %ls</li>", student.faculty);
			break;
		case 50:
			if (IsInArray(choice, nChoice, 5))
				fwprintf(fout, L"\t\t\t\t\t\t\t\t<li>Ngày sinh: %ls</li>", student.birthday);
			break;
		case 51:
			if (IsInArray(choice, nChoice, 4))
				fwprintf(fout, L"\t\t\t\t\t\t\t\t<li>Email: %ls</li>", student.email);
			break;
		case 57:
			if (IsInArray(choice, nChoice, 7))
				for (int i = 0; i < nHobby; i++)
					fwprintf(fout, L"\t\t\t\t\t\t\t\t<li>%ls</li>", student.hobby[i]);
			break;
		case 62:
			if (IsInArray(choice, nChoice, 6))
				fwprintf(fout, L"\t\t\t\t\t\t\t%ls", student.bio);
			break;
		}
		fseek(fout, ftell(fout), SEEK_SET);
	}
}

void HTMLGenerate(FILE *HTMLSource, STUDENT student, int *choice, int nChoice, int nHobby)
{
	FILE *fout = NULL;
	wchar_t filename[30] = L"WEBSITE\\"; //Create name for HTML file
	wcscat_s(filename, student.no);
	wcscat_s(filename, L".html");
	fout = _wfopen(filename, L"w+b, ccs=UTF-8");

	FileCopy(HTMLSource, fout); //Copy from HTML source file to HTML file

	HTMLEdit(fout, student, choice, nChoice, nHobby); //Edit HTML file

	fclose(fout);
}

int UserChoice(int *&choice, int &CSVformat, int &EmailField)
{
	bool error; //Error flag
	do
	{
		wprintf(L"\nCHOOSE YOUR CVS FILE FORMAT (DEFAULT: 1)\n");
		wprintf(L"0. \"Field 1\",\"Field 2\",\"Field 3\",...\n");
		wprintf(L"1. Field 1,Field 2,Field 3,...\n");
		wprintf(L"Your choice: ");
		wscanf(L"%d", &CSVformat);
		if (CSVformat == 0 || CSVformat == 1)
			error = false;
		else {
			wprintf(L"\nINVALID CHOICE!\n");
			error = true;
		}
	} while (error == true);

	do
	{
		wprintf(L"\nDoes your file have the \"EMAIL\" field? (DEFAULT: 1)\n");
		wprintf(L"0. No\n");
		wprintf(L"1. Yes\n");
		wprintf(L"Your choice: ");
		wscanf(L"%d", &EmailField);
		if (EmailField == 0 || EmailField == 1)
			error = false;
		else {
			wprintf(L"\nINVALID CHOICE!\n");
			error = true;
		}
	} while (error == true);

	int nChoice;
	do
	{
		wprintf(L"\nCUSTOMIZE THE PROFILE PAGES\n");
		wprintf(L"0. DEFAULT (SHOW ALL)\n");
		wprintf(L"1. Fullname\n");
		wprintf(L"2. Student number\n");
		wprintf(L"3. Faculty\n");
		if (EmailField == 1)
			wprintf(L"4. Email\n");
		wprintf(L"5. Birthday\n");
		wprintf(L"6. Bio\n");
		wprintf(L"7. Hobby\n");
		wprintf(L"\nHOW MANY fields to show in the profile page? ");
		wscanf(L"%d", &nChoice);

		if (nChoice >= 0 && nChoice <= 7)
			error = false;
		else {
			wprintf(L"\nINVALID CHOICE!\n");
			error = true;
		}
	} while (error == true);

	if (nChoice == 0) //Default mode
	{
		nChoice = 7;
		choice = (int*)malloc(nChoice * sizeof(int));
		for (int i = 0; i < nChoice; i++)
		{
			if (EmailField == 0 && i == 3) //If there is no email field, eliminate choice number 4 (email)
				continue;
			choice[i] = i + 1;
		}
	}
	else //Custom mode
	{
		do
		{
			choice = (int*)malloc(nChoice * sizeof(int));
			wprintf(L"Input the fields number:\n");
			for (int i = 0; i < nChoice; i++)
			{
				wscanf(L"%d", &choice[i]);
				if (choice[i] >= 1 && choice[i] <= 7)
					error = false;
				else {
					wprintf(L"\nINVALID CHOICE!\n");
					error = true;
					break;
				}
			}
		} while (error == true);
	}
	return nChoice;
}