#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

const int maxtablesize = 7;
const int T = 2;
const char *msgs[] = { "   0. Выход", "   1. Добавить элемент", "   2. Показать таблицу", "   3. Найти элемент по заданному ключу", "   4. Удалить элемент по заданному ключу" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

struct Node {                  
	int key[3];
	char* info[3];
	struct Node *ptr[4];
	struct Node *parent;
};

struct Item {
	int key;
	char* info;
	Node* tptr;
};

struct Tree {
	struct Node* root;              // корень дерева
	int m;                          //количество узлов корня
};

struct Kash {
	Item tab[maxtablesize];
};

void init(Tree*);
void printtree(Tree*);
int* getInt(int*);
int* getIntKey(int*);
char* getstr();


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Kash Kash;
	Tree Tree;
	init(&Tree);
	printtree(&Tree);
	return 0;
}

int* getInt(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr);
		if ((n == 0) || (*adr<0) || (*adr>(NMsgs - 1)))
		{
			printf_s("   Ошибка. Повторите ввод \n   ");
			scanf_s("%*[^\n]");
		}
	} while ((n == 0) || (*adr<0) || (*adr>(NMsgs - 1)));
	scanf_s("%*c");
	return adr;
}

int* getIntKey(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr);
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

void init(Tree* ukaz) {
	ukaz->m = 0;
	ukaz->root = (Node*)calloc(1, sizeof(Node));
	for (int i = 0; i < 4; i++) {
		ukaz->root->ptr[i] = (Node*)calloc(1, sizeof(Node));
	}
}

void printtree(Tree* ukaz) {
	printf_s("Ну вот дерево тип: %p\t%p\t%p\t%p\n", ukaz->root->ptr[0], ukaz->root->ptr[1], ukaz->root->ptr[2], ukaz->root->ptr[3]);
	printf_s("Размер структуры Node %d байт", sizeof(Node));
}
