#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>;
#include <stdlib.h>;
#include <locale.h>;
#include <string.h>;
#include <windows.h>;
char *getstr(int &);
void out(char*);
char *deleteprobely(char*, int, int);

int main()
{
	SetConsoleCP(1251); // ¬вод с консоли в кодировке 1251
	SetConsoleOutputCP(1251);
	int length;
	char *ptr;
	char *newline;
	int n, add;
	setlocale(LC_ALL, "Rus");
	printf_s("¬ведите n. »менно n буквенные слова € буду выдел€ть\n");
	add = scanf_s("%d", &n);
	printf_s("¬ведите текст: дл€ перехода на новую строку нажмите Enter, чтобы закончить ввод, нажмите Ctrl+Z\n");
	while ((ptr = getstr(length)) != NULL) {
		newline = deleteprobely(ptr, length, n);
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
			strcat(ptr, " ");
			len++;
		}
	} while (n > 0);
	return ptr;
}

char *deleteprobely(char *stroka, int len, int nnn)
{
	char* ukaz1;
	char * newstroka = (char *)calloc(len, sizeof(char *));
	char * juststroka = strdup(stroka);
	ukaz1 = juststroka;
	char * buf = (char *)calloc(nnn, sizeof(char *));
	int lena=0;
	int n;
	while (*ukaz1!='\0') {
		int index1 = strspn(ukaz1, "' '"); // поиск номера первого попавшегос€ пробела
		ukaz1 += index1; //переход к непробельному символу
		if (*(ukaz1 + nnn) == ' ') { // если через nnn символов есть пробел, то
			strncpy(buf, ukaz1, nnn); //мы эти nnn символов записываем в буферный массив
			n = strcspn(buf, "(),_-+=?1234567890!@#$%^&*:./\' '[]{}"); // в буферном массиве 
			// ищем пробелы и Ќ≈буквы
			if (n>=nnn) { // если в буферном массиве только буквы, то
				strcat(newstroka, buf); // к новой строке добавл€ем скопированное слово
				strcat(newstroka, " "); // ну и раздел€ем пробелом
				index1 = strspn(ukaz1, "' '");
				lena += (nnn+1);
				ukaz1 += nnn;
			}
			else {
				ukaz1++;
			}
		}
		else
		{
			n = strcspn(ukaz1, "' '");
			ukaz1 += n; // пропускаем n символов, переход€ к следующему пробелу
		}
	}
	*(newstroka+(lena-1)) = '\0';
	newstroka = (char *)realloc(newstroka, (lena-1)*sizeof(char *));
	return newstroka;
}