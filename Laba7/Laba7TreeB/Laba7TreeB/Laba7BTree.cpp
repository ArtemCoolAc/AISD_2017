#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

const int maxtablesize = 7;
const char *msgs[] = { "   0. Выход", "   1. Добавить элемент",/* "   2. Показать упорядоченную таблицу",*/ "   3. Найти элемент по заданному ключу" }; // ,"   4. Удалить элемент по заданному ключу" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

struct Node {
	int n; //количество ключей в узле 
	int key[3]; //значения ключей
	char* info[3]; // информационные поля
	struct Node* ptr[4]; // массив указателей на дочерние узлы
	struct Node* parent; // указатель на родительский элемент
};

int dialog();
int* getInt(int*);
int* getIntKey(int*);
char* getstr();
void inittree(Node*);
int enterdata(Node**);
int findelem(Node**);
void erase(Node*);
int B_Tree_Search(Node*, int, int*, int*, Node**);
int B_Tree_Split(Node*, int);
int B_Tree_Insert(Node**, int);
int B_Tree_Insert_Nonfull(Node*, int);
int InsertElem(Node*, int, char*);

int(*choice[])(Node**) = { NULL, enterdata, /*printtable,*/ findelem /*,deleteelem*/ };

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Node* Tree = (Node*)calloc(1, sizeof(Node));
	int n;
	while (n = dialog()) {
		choice[n](&Tree);
	}
	erase(Tree);
	return 0;
}

int dialog() {
	int i; int k; int* choice;
	for (i = 0; i < NMsgs; i++) {
		printf_s("%s\n", msgs[i]);
	}
	printf_s("   Ваш выбор: ");
	choice = getInt(&k);
	printf_s("\n");
	return *choice;
}

int enterdata(Node** ukaz) {
	int k; int flag; Node* uukaz = *ukaz; //char* pchar=NULL; 
	do {
		printf_s("   Введите ключ\n   ");
		getIntKey(&k);
		//printf_s("   Введите данные\n   ");
		//pchar = getstr();
		printf_s("\n");
		Node* ptr=NULL;
		B_Tree_Search(uukaz, k, &flag, &flag, &ptr);
		if (!ptr) ptr = uukaz;
		flag = B_Tree_Insert(&ptr, k);
		//flag = B_Tree_Insert(&uukaz, k);
		if (flag == -1) printf_s("Ошибка! Дублирование ключей невозможно. Повторите попытку.\n");
	} while (flag == -1);
	*ukaz = uukaz;
	return 0;
}

int findelem(Node** ukaz) {
	int k; int flag; int next; int index; Node* ptr = (Node*)calloc(1, sizeof(Node));
	printf_s("   Введите ключ, по которому нужно отыскать элемент:\n   ");
	getIntKey(&k);
	flag = B_Tree_Search(*ukaz, k, &index, &next, &ptr);
	if (next)
	printf_s("   После элемента с ключом %d идет элемент с ключом %d\n   ", k, next);
	else printf_s("   Данный элемент - последний в дереве\n");
	return flag;
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
			scanf_s("%*c");
		}
	} while ((n == 0) || (*adr<0) || (*adr>(NMsgs - 1)));
	return adr;
}

int* getIntKey(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr);
		if (n == 0)
		{
			printf_s("   Ошибка. Повторите ввод \n   ");
			scanf_s("%*[^\n]");
		}
	} while (n == 0);
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


void erase(Node* ukaz) {
	for (int i = 0; i < 4; i++) {
		free(ukaz->ptr[i]);
		ukaz->ptr[i] = NULL;
	}
}

int B_Tree_Search(Node* ukaz, int ikey, int* index, int* next, Node** uptr) {
	Node* x = ukaz; int i; Node* par; 
	while (x) {
		for (i = 0; ((i < x->n) && (ikey>=x->key[i])); ++i) {
			if (x->key[i] == ikey) {
				*index = i;
				*uptr = x;
				if ((x->n!=3) && (x->key[i+1]>ikey)) *next = x->key[i + 1];
				else {
					par = x->parent;
					while (par) {
						if (par->key[par->n - 1] < ikey) {
							int j = 0;
							while ((j<par->n) && (par->key[j] < ikey)) {
								j++;
							}
							*next = par->key[j - 1];
							if (*next < ikey) *index = 0;
						}
						par = par->parent;
					}
					 
				}
				return *index;
			}
		}
		x = x->ptr[i];
	}
	if (!(ukaz->ptr[0])) *index = 0;
	else *index = i;
	return -1;
}

int B_Tree_Split(Node* x, int i) {
	int m = x->n;
	int n = 3; int d = 0;
	Node* y = x->ptr[i]; int ii = 3;
	Node* z = (Node*)calloc(1, sizeof(Node));
	int j = n / 2 + 1;
	int dif = y->n - j;
	for (d = 0; d < dif; d++) {
		z->key[d] = y->key[j + d];
		z->ptr[d] = y->ptr[j + d];
	}
	x->ptr[i + 1] = z;
	x->key[i] = y->key[n / 2];
	x->n++;
	z->n += d;
	y->key[1] = 0;
	y->key[2] = 0;
	y->n -= (d + 1);
	y->parent = x;
	z->parent = x;
	return 1;
}

int InsertElem(Node* ukaz, int ikey, char* iinfo) {
	Node* pointer = ukaz; int res; int m; int index=0;
	Node* x = NULL;
	Node* par = NULL; int next;
	res = B_Tree_Search(ukaz, ikey, &index, &next, &x);
	if (res == -1) {
		while (x->key[x->n - 1]) {
			m = (0 + x->n - 1) / 2;
			B_Tree_Split(x, index);
			par = x;
			x = x->parent;
		}
		return 1;
	}
	else 
	return -1;
}

int B_Tree_Insert(Node** ukaz, int ikey) {
	Node* root = *ukaz;
	Node* r = root;
	int res;

	if (r->n==3) {
		Node* s = (Node*)calloc(1, sizeof(Node));
		s->ptr[0] = r;
		root = s;
		B_Tree_Split(s, 0);
		r = s;
		*ukaz = root;
	}
	if ((res=B_Tree_Insert_Nonfull(r, ikey))==-1) return -1;
	else {
		return 1;
	}
}

int B_Tree_Insert_Nonfull(Node* x, int k) {
	int i; int j; Node* par = NULL; 
	
		while (x->ptr[0]) {
			for (i = 0; ((i < x->n) && (k>x->key[i])); ++i) {
			}
			if (x->key[i] == k) return -1;
			Node* s = x->ptr[i];
			if (s->n == 3) {
				B_Tree_Split(x, i);
				if (k > x->key[i])
					s = x->ptr[i + 1];
			}
			x = s;
		}
	for (j = 0; ((j < x->n) && (k>=x->key[j])); ++j) {
		if (x->key[j] == k) return -1;
	}
	int z = x->n;
	while (z - j) {
		x->key[z] = x->key[z-1];
		--z;
	}
	x->key[j] = k;
	x->n++;
	return 1;
}