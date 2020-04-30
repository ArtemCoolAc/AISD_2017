#include <stdio.h>;
#include <stdlib.h>;
#include <locale.h>;
#include <string.h>;
#include <windows.h>;

struct StrokaSpisok { 
	char *bukva;
	struct StrokaSpisok *next;
};

void CreateList(struct StrokaSpisok*);

void main()
{
	struct StrokaSpisok stroka;
	SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
	SetConsoleOutputCP(1251); // Вывод -.-
	printf_s("Введите строку\n");
	CreateList(&stroka);
 }

void CreateList(struct StrokaSpisok *ukaz)
{
	char *ptr = (char *)calloc(1, sizeof(char *));
	char *buf = (char *)calloc(81, sizeof(char *)); char* buf1;
	struct StrokaSpisok * head; struct StrokaSpisok * tmp;
	int n; int q = 0;
	*ptr = '\0';
//do {
	n = scanf_s("%80s", buf, 81); //n = scanf_s("%80[^\n]", buf); 
	buf1 = buf;
	/* if (n < 0) { // когда считан конец строки или файла
		continue;
	}
	if (n == 0)
		scanf_s("%*c");
}

while (n > 0); */

if (*buf == '\0') printf_s("");
else {
	if (*(++buf1) == '\0') {
		//head = NULL;
		*(head->bukva) = *buf;
	}
	else {
		//head = NULL;
		*(ukaz->bukva) = *buf; head = ukaz;
		ukaz->next = NULL; q = 1;
		while ((*buf1) != '\0')
		{
			struct StrokaSpisok *tmp = (struct StrokaSpisok*)malloc(sizeof(struct StrokaSpisok));
			*(ukaz->bukva) = *buf1;
			tmp->next = NULL;
			ukaz->next = tmp;
			q++;
			buf1++;
		}
	}
}
}