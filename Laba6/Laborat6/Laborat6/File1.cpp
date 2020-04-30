#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

int* getIntKey(int*);
char* getstr();

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	FILE *fd;

	//char fname[] = "files.dat";
	printf_s("Введите имя файла\n");
	char* fname = getstr();
	printf_s("Введите строку\n");
	char* inbuf = getstr();
	int len = strlen(inbuf);
	//char inbuf[] = "data for testing";
	char outbuf[20];
	long size;
	fd = fopen(fname, "w+b");
	if (fd == NULL) {
	fprintf(stderr, "can't open file %s\n", fname);
		return 1;
	}
	printf("size of data: %d\n", len);
	fwrite(inbuf, 1, len, fd);
	fseek(fd, 1, SEEK_CUR);
	fwrite(inbuf, 1, len, fd);
	fseek(fd, 0, SEEK_END);
	size = ftell(fd);
	printf("File %s has %ld bytes\n", fname, size);
	rewind(fd);
	printf("reading from file\n");
	while (size = fread(outbuf, 1, sizeof(outbuf), fd),size > 0)
	fwrite(outbuf, 1, size, stdout);
	printf("\n");
	fclose(fd);
	return 0;
}


int* getIntKey(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr, 4);
		if ((n == 0) || (*adr <= 0))
		{
			printf_s("   Ошибка. Повторите ввод \n   ");
			scanf_s("%*[^\n]");
		}
	} while ((n == 0) || (*adr <= 0));
	scanf_s("%*c");
	return adr;
}

char *getstr()
{
	char *ptr = (char *)calloc(1, sizeof(char *));
	char buf[81];
	int n;
	int len = 0;
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf); //n = scanf_s("%80[^\n]", buf); 
		if (n < 0) { // когда считан конец строки или файла
			free(ptr); // освобождаем память
			ptr = NULL;
			continue;
		}
		if (n == 0)
			scanf_s("%*c");
		else {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 1);
			strcat(ptr, buf); //из буферного в строку птр
		}
	} while (n > 0);
	return ptr;
}