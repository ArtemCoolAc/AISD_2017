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
	SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
	SetConsoleOutputCP(1251); // Вывод -.-
	int length;
	char *ptr;
	char *newline;
	int n, add;
	setlocale(LC_ALL, "Rus");
	printf_s("Введите n. Именно n буквенные слова я буду выделять\n");
	getInt(&n);
	printf_s("Введите текст: для перехода на новую строку нажмите Enter, чтобы закончить ввод, нажмите Ctrl+Z\n");
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
			printf_s("Ошибка. Повторите ввод \n");
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
		if (n < 0) { // когда считан конец строки или файла
			free(ptr); // освобождаем память
			ptr = NULL;
			continue;
		}
		if (n == 0)
			scanf_s("%*c");
		else {
			len += strlen(buf); 
			ptr = (char *)realloc(ptr, len + 2); // память ещё и для пробела
			strcat(ptr, buf); //из буферного в строку птр
			strcat(ptr, " "); // и пробел, который мне нужен
			len++; // из-за пробела +1
		}
	} while (n > 0);
	return ptr;
}

char *deleteprobely(char *stroka, int len, int nnn)
{
	char * newstroka = (char *)calloc(len, sizeof(char *));
	char * juststroka = strdup(stroka); // дублируем исходную строку, чтобы её не потерять
	char * ukaz1 = juststroka; // указатель на копию исходной строки
	char * buf = (char *)calloc(nnn, sizeof(char *)); // память под буферный массив
	int lena = 0;
	int n;
	while (*ukaz1 != '\0') { //пока не дойдем до конца строки
		int index1 = strspn(ukaz1, "' '\t"); // ищем номер первого непробельного символа
		ukaz1 += index1; //переходим к непробельному символу
		if (*(ukaz1 + nnn) == ' ') { // если через nnn символов есть пробел, то
			strncpy(buf, ukaz1, nnn); //мы эти nnn символов записываем в буферный массив
			n = strcspn(buf, "(),_-+=?1234567890!@#$%^&*:;~`№.|/\' '\t[]{}"); // в буферном массиве 
																	   // ищем пробелы и НЕбуквы
			if (n >= nnn) { // если в буферном массиве только буквы, то
				strcat(newstroka, buf); // к новой строке добавляем скопированное слово
				strcat(newstroka, " "); // ну и разделяем пробелом
				lena += (nnn + 1); //увеличиваем длину строки с учетом пробела
				ukaz1 += nnn; // пропускаем слово
			}
			else {
				ukaz1++; // если нашли НЕбукву, переместим указатель на следующий элемент, а то зациклится
			}
		}
		else
		{
			n = strcspn(ukaz1, "' '\t"); // вычисляем следующий пробел
			ukaz1 += n; // пропускаем n символов, переходя к следующему пробелу
		}
	}
	*(newstroka + (lena - 1)) = '\0'; // на месте последнего символа нуль-ограничитель (был пробел)
	newstroka = (char *)realloc(newstroka, (lena - 1) * sizeof(char *));
	return newstroka;
}