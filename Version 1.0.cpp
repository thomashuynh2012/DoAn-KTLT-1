#include <stdio.h>
#include <cwchar>
#include <conio.h>
#include <fcntl.h>
#include <io.h>

struct STUDENT
{
	wchar_t no[11];
	wchar_t name[30];
	wchar_t faculty[30];
	wchar_t email[50];
	int year;
	wchar_t birthday[11];
	wchar_t image[100];
	wchar_t bio[1000];
	wchar_t hobby[100];
};

STUDENT ReadFile(FILE *fp)
{
	STUDENT s;
	fwscanf(fp, L" \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%d  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]",
		s.no, s.name, s.faculty, s.email, &s.year, s.birthday, s.image, s.bio, s.hobby);
	fgetwc(fp);

	return s;
}

void ShowSTUDENT(STUDENT s)
{
	wprintf(L"Number: %s\n", s.no);
	wprintf(L"Name: %ls\n", s.name);
	wprintf(L"Faculty: %ls\n", s.faculty);
	wprintf(L"Email: %ls\n", s.email);
	wprintf(L"Academic year: %d\n", s.year);
	wprintf(L"Birthday: %s\n", s.birthday);
	wprintf(L"Image link: %s\n", s.image);
	wprintf(L"Bio: %ls\n", s.bio);
	wprintf(L"Hobby: %ls", s.hobby);
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	wchar_t *InFile = L"students.txt";
	FILE *fin = NULL;
	_wfopen_s(&fin, InFile, L"r, ccs=UTF-8");

	int nStudent = 2;
	STUDENT *s = new STUDENT[nStudent];
	for (int i = 0; i < nStudent; i++)
	{
		s[i] = ReadFile(fin);
		ShowSTUDENT(s[i]);
		wprintf(L"\n\n");
	}

	fclose(fin);
	delete[] s;
	_getch();
}
