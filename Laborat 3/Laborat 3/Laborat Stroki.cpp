#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>;
#include <stdlib.h>;
#include <locale.h>;
#include <string.h>;
#include <windows.h>;


char *getstr(int &);
void out(char*);
char *deleteprobely(char*, int);

int main()
{
	SetConsoleCP(1251); // ¬вод с консоли в кодировке 1251
	SetConsoleOutputCP(1251);
	int length;
	char *ptr;
	char *newline;
	setlocale(LC_ALL, "Rus");
	printf_s("¬ведите текст: дл€ перехода на новую строку нажмите Enter, чтобы закончить ввод, нажмите Ctrl+Z\n");
	while ((ptr = getstr(length))!=NULL) {
		out(ptr);
		newline=deleteprobely(ptr,length);
		out(newline);
		free(ptr);
	}
	return 0;
}

void out(char *rab)
{
printf_s("\"%s\"\n", rab);
}

char *getstr(int &len)
{
	char *ptr = (char *)calloc(1, sizeof(char *));
	char buf[81]; 
	int n; 
	len = 0;
	*ptr = '\0';
	do{
		n = scanf_s("%80[^\n]", buf, 81); //n = scanf_s("%80[^\n]", buf); 
		if (n < 0){
			
			free(ptr);
			ptr = NULL;
			continue;
		}

		if (n == 0)
			scanf_s("%*c");
		else {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 2);
			strcat(ptr, buf);
			*(ptr + len + 1) = ' ';
			len++;
		}
	} while (n > 0);
	return ptr;
}

char *deleteprobely(char *stroka, int len)
{
	char* ukaz1 = stroka; 
	char* ukaz2 = stroka;
	char buf[4] = { 0 };
	char probel[1] = { ' ' };
	char * newstroka = (char *)calloc(len, sizeof(char *));
	int lena=0;
	int lencopy = len;
	int index1;
	int n;
	
	index1 = strspn(ukaz1, "[' ']");
	while (*ukaz1!='\0') {
		index1 = strspn(ukaz1, "[' ']");
		ukaz1 += index1;
		ukaz2 = ukaz1;
		if (*(ukaz1 + 3) == ' ') {
			strncpy(buf, ukaz2, 3);
			 n = strcspn(buf, "[' ']");
			if (n != 2 && n!=0) {
				
				buf[4] = '\0';
				strcat(newstroka, buf);
				strcat(newstroka, probel);
				lencopy -= index1;
				index1 = strspn(ukaz1, "[' ']");
				lena += 4;
				ukaz1 += 3;
				ukaz2 += 3;
			}
			else {
				ukaz1++;
				ukaz2 = ukaz1;
			}
		}
		else
		{
			n = strcspn(ukaz1, "[' ']");
			ukaz1 += n;
			ukaz2 = ukaz1;
		}

	}
	newstroka = (char *)realloc(newstroka, lena*sizeof(char *));
	return newstroka;
}