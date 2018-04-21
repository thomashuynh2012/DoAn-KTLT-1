#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <cwchar>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#define MAX_LINES 1000

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

int CountHobby(wchar_t *ch)
{
	int count = 0;
	for (int i = 0; i < wcslen(ch); i++)
	{
		if (ch[i] == L'"')
			count++;
	}
	return count/2;
}

STUDENT ReadFile(FILE *fp, int &nHobby)
{
	STUDENT s;
	fwscanf(fp, L" \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%d  \",  \"%[^\",]  \",  \"%[^\",]  \",  \"%[^\",]  ",
		s.no, s.name, s.faculty, s.email, &s.year, s.birthday, s.image, s.bio);

	s.hobby = new wchar_t*[1];
	s.hobby[0] = new wchar_t[1000];
	int pos = ftell(fp);
	fwscanf(fp, L"\",%[^\n]", s.hobby[0]);
	fgetwc(fp);

	nHobby = CountHobby(*s.hobby);
	if (nHobby > 1)
	{
		delete[] s.hobby[0];
		delete[] s.hobby;
		s.hobby = new wchar_t*[nHobby];
		for (int i = 0; i < nHobby; i++)
		{
			s.hobby[i] = new wchar_t[100];
		}

		fseek(fp, pos, SEEK_SET);
		for (int i = 0; i < nHobby; i++)
			fwscanf(fp, L"\",\"%[^\"\n]", s.hobby[i]);
		fgetwc(fp);
	}
	else if (nHobby == 1)
	{
		fseek(fp, pos, SEEK_SET);
		fwscanf(fp, L"\",\"%[^\"\n]", s.hobby[0]);
		fgetwc(fp);
	}
	return s;
}

void ShowSTUDENT(STUDENT s, int nHobby)
{
	wprintf(L"Number: %s\n", s.no);
	wprintf(L"Name: %ls\n", s.name);
	wprintf(L"Faculty: %ls\n", s.faculty);
	wprintf(L"Email: %ls\n", s.email);
	wprintf(L"Academic year: %d\n", s.year);
	wprintf(L"Birthday: %s\n", s.birthday);
	wprintf(L"Image link: %s\n", s.image);
	wprintf(L"Bio: %ls\n", s.bio);
	for (int i = 0; i < nHobby; i++)
		wprintf(L"Hobby: %ls\n", s.hobby[i]);
}

void FileCopy(FILE *fin, FILE *fout)
{
	rewind(fin);
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

bool IsInArray(int *a, int A, int number)
{
	for (int i = 0; i < A; i++)
		if (a[i] == number)
			return true;

	return false;
}

void HTMLEdit(FILE* fout, STUDENT student, int *choice, int nChoice, int nHobby)
{
	wchar_t title[60] = L"<title>HCMUS - ";
	wcscat_s(title, student.name);
	wcscat_s(title, L"</title>");

	wchar_t fullname_head[80] = L"<span class=\"Personal_FullName\">";
	wcscat_s(fullname_head, student.name);
	wcscat_s(fullname_head, L" - ");
	wcscat_s(fullname_head, student.no);
	wcscat_s(fullname_head, L"</span>");

	wchar_t faculty_head[80] = L"<div class=\"Personal_Department\">Khoa ";
	wcscat_s(faculty_head, student.faculty);
	wcscat_s(faculty_head, L"</div>");

	wchar_t email_head[60] = L"Email: ";
	wcscat_s(email_head, student.email);

	wchar_t image_head[80] = L"<img src = \"Images/";
	wcscat_s(image_head, student.image);
	wcscat_s(image_head, L"\" class = \"Personal_Hinhcanhan\" / >");

	wchar_t fullname[51] = L"<li>Họ và tên: ";
	wcscat_s(fullname, student.name);
	wcscat_s(fullname, L"</li>");

	wchar_t number[26] = L"<li>MSSV: ";
	wcscat_s(number, student.no);
	wcscat_s(number, L"</li>");

	wchar_t faculty[70] = L"<li>Sinh viên khoa: ";
	wcscat_s(faculty, student.faculty);
	wcscat_s(faculty, L"</li>");

	wchar_t birthday[31] = L"<li>Ngày sinh: ";
	wcscat_s(birthday, student.birthday);
	wcscat_s(birthday, L"</li>");

	wchar_t email[70] = L"<li>Email: ";
	wcscat_s(email, student.email);
	wcscat_s(email, L"</li>");

	wchar_t hobby[1000] = L"<li>";
	wcscat_s(hobby, *student.hobby);
	wcscat_s(hobby, L"</li>");

	wchar_t bio[1040] = L"";
	wcscat_s(bio, student.bio);

	rewind(fout);
	int count = 1;
	wchar_t line[MAX_LINES];
	while (fgetws(line, MAX_LINES - 1, fout) != NULL)
	{
		count++;

		fseek(fout, ftell(fout), SEEK_SET);
		switch (count)
		{
		case 6:
			fputws(title, fout); break;
		case 23:
			if (IsInArray(choice, nChoice, 1))
				fputws(fullname_head, fout); 
			break;
		case 24:
			if (IsInArray(choice, nChoice, 3))
				fputws(faculty_head, fout); 
			break;
		case 27:
			if (IsInArray(choice, nChoice, 5))
				fputws(email_head, fout); 
			break;
		case 34:
			fputws(image_head, fout); break;
		case 47:
			if (IsInArray(choice, nChoice, 1))
				fputws(fullname, fout);
			break;
		case 48:
			if (IsInArray(choice, nChoice, 2))
				fputws(number, fout);
			break;
		case 49:
			if (IsInArray(choice, nChoice, 3))
				fputws(faculty, fout);
			break;
		case 50:
			if (IsInArray(choice, nChoice, 4))
				fputws(birthday, fout);
			break;
		case 51:
			if (IsInArray(choice, nChoice, 5))
				fputws(email, fout);
			break;
		case 57:
			if (IsInArray(choice, nChoice, 6))
				fputws(hobby, fout);
			break;
		case 82:
			if (IsInArray(choice, nChoice, 7))
				fputws(bio, fout);
			break;
		}
		fseek(fout, ftell(fout), SEEK_SET);
	}
}

void HTMLGenerate(FILE *HTMLSource, STUDENT student, int *choice, int nChoice, int nHobby)
{
	FILE *fout = NULL;
	wchar_t filename[15];
	wcscpy_s(filename, student.no);
	wcscat_s(filename, L".html");
	_wfopen_s(&fout, filename, L"w+b, ccs=UTF-8");

	FileCopy(HTMLSource, fout);

	HTMLEdit(fout, student, choice, nChoice, nHobby);
	fclose(fout);
}

int UserChoice(int *&choice)
{
	int nChoice;
	wprintf(L"0. DEFAULT (SHOW ALL)\n");
	wprintf(L"1. Fullname\n");
	wprintf(L"2. Student number\n");
	wprintf(L"3. Faculty\n");
	wprintf(L"4. Birthday\n");
	wprintf(L"5. Email\n");
	wprintf(L"6. Bio\n");
	wprintf(L"7. Hobby\n");
	wprintf(L"How many fields to show in the profile page? ");
	wscanf_s(L"%d", &nChoice);

	if (nChoice == 0)
	{
		nChoice = 7;
		choice = new int[nChoice];
		for (int i = 0; i < nChoice; i++)
		{
			choice[i] = i + 1;
		}
	}
	else
	{
		choice = new int[nChoice];
		wprintf(L"Input the fields number:\n");
		for (int i = 0; i < nChoice; i++)
		{
			wscanf_s(L"%d", &choice[i]);
		}
	}
	return nChoice;
}

void main()
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	wchar_t *StudentFile = L"students.csv";
	FILE *fin = NULL;
	_wfopen_s(&fin, StudentFile, L"r, ccs=UTF-8");

	wchar_t *HTMLFile = L"HTMLSource2.txt";
	FILE *HTMLSource = NULL;
	_wfopen_s(&HTMLSource, HTMLFile, L"r, ccs=UTF-8");

	int *choice = NULL;
	int nChoice = UserChoice(choice);

	int nStudent = 4;
	int nHobby;
	STUDENT *student = new STUDENT[nStudent];
	for (int i = 0; i < nStudent; i++)
	{
		wprintf(L"\n");
		student[i] = ReadFile(fin, nHobby);
		ShowSTUDENT(student[i], nHobby);
		HTMLGenerate(HTMLSource, student[i], choice, nChoice, nHobby);
	}

	fclose(HTMLSource);
	fclose(fin);
	delete[] student;
	delete[] choice;
	_getch();
}