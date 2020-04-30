#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>;
#include <stdlib.h>;
#include <locale.h>;
#include <string.h>;
#include <windows.h>;

char *getstr(int&);
void out(char*);
char *deleteprobely(char*, int, int);
void getInt(int*);

int main()
{
	SetConsoleCP(1251); // ���� � ������� � ��������� 1251
	SetConsoleOutputCP(1251); // ����� -.-
	int length;
	char *ptr;
	char *newline;
	int n, add;
	setlocale(LC_ALL, "Rus");
	printf_s("������� n. ������ n ��������� ����� � ���� ��������\n");
	getInt(&n);
	printf_s("������� �����: ��� �������� �� ����� ������ ������� Enter, ����� ��������� ����, ������� Ctrl+Z\n");
	while ((ptr = getstr(length)) != NULL) {
		newline = deleteprobely(ptr, length, n);
		out(newline);
		free(ptr);
		free(newline);
	}
	return 0;
}

void out(char *rab)
{
	printf_s("\"%s\"\n", rab);
}

void getInt(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr, 4);
		if (n == 0)
		{
			printf_s("������. ��������� ���� \n");
			scanf_s("%*[^\n]");
		}
	} while (n == 0);
}

char *getstr(int &len)
{
	char *ptr = (char *)calloc(1, sizeof(char *));
	char buf[81];
	int n;
	len = 0;
	*ptr = '\0';
	do {
		n = scanf_s("%80[^\n]", buf, 81); //n = scanf_s("%80[^\n]", buf); 
		if (n < 0) { // ����� ������ ����� ������ ��� �����
			free(ptr); // ����������� ������
			ptr = NULL;
			continue;
		}
		if (n == 0)
			scanf_s("%*c");
		else {
			len += strlen(buf); 
			ptr = (char *)realloc(ptr, len + 2); // ������ ��� � ��� �������
			strcat(ptr, buf); //�� ��������� � ������ ���
			strcat(ptr, " "); // � ������, ������� ��� �����
			len++; // ��-�� ������� +1
		}
	} while (n > 0);
	return ptr;
}

char *deleteprobely(char *stroka, int len, int nnn)
{
	char * newstroka = (char *)calloc(len, sizeof(char *));
	char * juststroka = strdup(stroka); // ��������� �������� ������, ����� � �� ��������
	char * ukaz1 = juststroka; // ��������� �� ����� �������� ������
	char * buf = (char *)calloc(nnn, sizeof(char *)); // ������ ��� �������� ������
	int lena = 0;
	int n;
	while (*ukaz1 != '\0') { //���� �� ������ �� ����� ������
		int index1 = strspn(ukaz1, "' '\t"); // ���� ����� ������� ������������� �������
		ukaz1 += index1; //��������� � ������������� �������
		if (*(ukaz1 + nnn) == ' ') { // ���� ����� nnn �������� ���� ������, ��
			strncpy(buf, ukaz1, nnn); //�� ��� nnn �������� ���������� � �������� ������
			n = strcspn(buf, "(),_-+=?1234567890!@#$%^&*:;~`�.|/\' '\t[]{}"); // � �������� ������� 
																	   // ���� ������� � �������
			if (n >= nnn) { // ���� � �������� ������� ������ �����, ��
				strcat(newstroka, buf); // � ����� ������ ��������� ������������� �����
				strcat(newstroka, " "); // �� � ��������� ��������
				lena += (nnn + 1); //����������� ����� ������ � ������ �������
				ukaz1 += nnn; // ���������� �����
			}
			else {
				ukaz1++; // ���� ����� �������, ���������� ��������� �� ��������� �������, � �� ����������
			}
		}
		else
		{
			n = strcspn(ukaz1, "' '\t"); // ��������� ��������� ������
			ukaz1 += n; // ���������� n ��������, �������� � ���������� �������
		}
	}
	*(newstroka + (lena - 1)) = '\0'; // �� ����� ���������� ������� ����-������������ (��� ������)
	newstroka = (char *)realloc(newstroka, (lena - 1) * sizeof(char *));
	return newstroka;
}