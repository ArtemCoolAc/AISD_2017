#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <malloc.h>
#include <locale.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define T 2
#define N 1000

typedef struct Item {
	int key;
	char* info;
}Item;
typedef struct Node {
	int n;
	Item element[2 * T - 1];
	struct Node* ptr [2 * T];
} Node;
typedef struct Table {
	int key;
	Node* p;
} Table;
int DelTabItem(Table*, int);
void TreeSplit(Node*, int, Table*);
void InsertTable(int, Node*, Table*);
int InsertTree(Node**, Table*, int, char*);
int DefineLeaf(Node*);
int InsertNonfull(Node*, int, char*, Table*);
Node* FindMax(Node*, int*);
Node* FindMin(Node*, int*);
int DelItem(Node**, int, Table*);
void PrintTree(Node*, int);
void TreeOut(Node*);
void DelTree(Node* );
Node* Find(Node*, int, Table*, int*);
int getInt(int*pa);
char *getstr();
int Add(Node**, Table*);
int Delete(Node**, Table*);
int Show(Node**, Table*);
int Print(Node**, Table*);
int Search(Node**, Table*);
int Max(Node **, Table*);
int End(Node**, Table*);
int Dialog(char*msg[], int n);
int AddRand(Node**, Table *);
void TreeSplit(Node* x, int i, Table* tbl)
{
	Node* y = x->ptr[i], *z = NULL;
	int j, n = 2 * T - 1, k = 0;
	for (j = (2 * T - 2); j > i; --j)
		x->element[j] = x->element[j - 1];
	for (j = (2 * T - 1); j > i + 1; --j)
		x->ptr[j] = x-> ptr[j - 1];
	z = (Node*)calloc(1, sizeof(Node));
	z->n = 0;
	for (j = (n / 2 + 1); j <= (2 * T - 2); ++j) {
		z->element[k] = y->element[j];
		InsertTable(z->element[k].key, z, tbl);
		++k;
		y->n--;
		z->n++;
	}
	k = 0;
	for (j = (n / 2 + 1); j <= (2 * T - 1); ++j) {
		z->ptr[k] = y->ptr[j];
		++k;
		y->ptr[j] = NULL;
	}
	x->ptr[i+1] = z;
	x->element[i] = y->element[n / 2];
	InsertTable(x->element[i].key, x, tbl);
	x->n++;
	y->n--;

}
void InsertTable(int k, Node*pp, Table* tbl) {
	tbl[k % N].key = k;
	tbl[k % N].p = pp;
}
int InsertTree(Node** root, Table* tbl, int key, char*info) {
	Node*r = *root, *s=NULL;
	int a;
	if ((tbl[key%N].key == key)&&(tbl[key%N].p!=NULL))
		return 1;              // дублирование ключей
	if (r->n == 3)
	{
		s = (Node*)calloc(1, sizeof(Node));
		s->n = 0;
		s->ptr[0] = r;
		*root = s;
		TreeSplit(s, 0, tbl);
		r = s;
	}
	a = InsertNonfull(r, key, info, tbl);
	return a;
}
int DefineLeaf(Node*r)
{
	if ((r->ptr[0] == NULL) && (r->ptr[1] == NULL) && (r->ptr[2] == NULL) && (r->ptr[0] == NULL))
		return 0;     // указанный узел является листом
	else
		return 1;      // указанный узел не лист
}
int DelTabItem(Table* tbl, int k) {
	if (tbl[k%N].key == k)
		tbl[k%N].p = NULL;
	return 1;
}
int InsertNonfull(Node*x, int k, char*info, Table* tbl )
{
	Node* s = NULL;
	int i = 0, j;
	while (DefineLeaf(x)) {
		while ((i<(x->n))&&(k > x->element[i].key))
			++i;
		if ((i<x->n)&&(k == x->element[i].key))
			return 1;      //дублирование ключей
		s = x->ptr[i];
		if (s->n == 3) {
			TreeSplit(x, i, tbl);
			if (k > x->element[i].key)
				s = x->ptr[i + 1];
		}
		x = s;
		i = 0;
}
	while ((i<x->n)&&(k > x->element[i].key))
		++i;
	if ((i<x->n)&&(k == x->element[i].key))
		return 1;      //дублирование ключей
	for (j = (2 * T - 2); j > i; --j)
		x->element[j] = x->element[j - 1];
	x->element[i]. key = k;
	x->element[i].info = _strdup(info);
	x->n++;
	InsertTable(k, x, tbl);
	return 0;
}
Node* FindMax(Node* x, int* a)
{
	if (!DefineLeaf(x))
		if (x->n == 0)
			*a = -1;
		else
			*a = x->n - 1;
	while (DefineLeaf(x))
		x = x->ptr[x->n];
	*a = (x->n)-1;
	return x;
}
Node* FindMin(Node* x, int* a)
{
	if (!DefineLeaf(x))
		if (x->n != 0)
			*a = 0;
	while (DefineLeaf(x))
		x = x->ptr[0];
	*a = 0;
	return x;
}
int DelItem(Node**r, int k, Table* tbl)

{
	Node*x = *r, *y = NULL, *z = NULL, *z1 = NULL;
	Node* q;
	int i = 0, j, h, fl;
	if (x->n == 1) {
		y = x->ptr[0];
		z = x->ptr[1];
		if (x && y && (y->n == 1) && (z->n == 1))
		{
			y->element[1] = x->element[0];
			y->n++;
			InsertTable(y->element[1].key, y, tbl);
			y->element[2] = z->element[0];
			y->n++;
			InsertTable(y->element[2].key, y, tbl);
			y->ptr[2] = z->ptr[0];
			y->ptr[3] = z->ptr[1];
			*r = y;
			free(x);
			free(z);
			x = y;
		}
	}
	while (DefineLeaf(x)) {
		i = 0;
		while ((i < x->n) && (k > x->element[i].key))
			++i;
		if ((i == x->n) || (k != x->element[i].key))
		{
			y = x->ptr[i];
			if (i == 0)
			{
				z = x->ptr[1];                                    // есть только сосед справа
				if ((y->n == (T - 1)) && (z->n >= T))
				{
					y->element[1] = x->element[i];
					InsertTable(y->element[1].key, y, tbl);
					x->element[i] = z->element[0];
					InsertTable(x->element[i].key, x, tbl);
					y->ptr[2] = z->ptr[0];
					y->n++;
					z->n--;
					//переместить элементы
					for (j = 0; j < z->n; ++j)                  
						z->element[j] = z->element[j + 1];
					for (j = 0; j <= z->n; ++j)
						z->ptr[j] = z->ptr[j + 1];
					//z->ptr[j] = NULL;
				}
				else if ((y->n == (T - 1)) && (z->n == (T - 1)))
				{
					y->element[1] = x->element[i];
					y->n++;
					x->n--;
					InsertTable(y->element[1].key, y, tbl);
					for (j = i; j < x->n; ++j)
					{
						x->element[j] = x->element[j + 1];
						x->ptr[j + 1] = x->ptr[j + 2];
					}
					//x->ptr[j + 1] = NULL;
					y->n++;
					y->element[2] = z->element[0];
					InsertTable(y->element[2].key, y, tbl);
					y->ptr[2] = z->ptr[0];
					y->ptr[3] = z->ptr[1];
					free(z);
				}
			}
			if (i == x->n)
			{
				z = x->ptr[i-1];       //    есть только левый сосед
				if ((y->n == (T - 1)) && (z->n >= T))
				{
					y->element[1] = y->element[0];
					y->ptr[2] = y->ptr[1];
					y->ptr[1] = y->ptr[0];
					y->n++;
					y->element[0] = x->element[x->n-1];
					x->element[x->n-1] = z->element[(z->n) - 1];
					z->n--;
					InsertTable(y->element[0].key, y, tbl);
					InsertTable(x->element[x->n - 1].key, x, tbl);
					y->ptr[0] = z->ptr[z->n+1 ];
					//z->ptr[z->n + 1] = NULL;
				}
				else if ((y->n == (T - 1)) && (z->n == (T - 1))) {

					z->element[1] = x->element[x->n-1];
					z->n++;
					x->n--;
					InsertTable(z->element[1].key, z, tbl);
					//x->ptr[x->n+1] = NULL;
					z->n++;
					z->element[2] = y->element[0];
					InsertTable(z->element[2].key, z, tbl);
					z->ptr[2] = y->ptr[0];
					z->ptr[3] = y->ptr[1];
					free(y);
					y = z;
				}
			}
			if ((i > 0) && (i < x->n)) {
				z = x->ptr[i - 1];         //левый сосед
				z1 = x->ptr[i + 1];       //правый сосед
				if (((y->n == (T - 1)) && (z->n >= T)) || ((y->n == (T - 1)) && (z1->n >= T))) {
					if ((y->n == (T - 1)) && (z->n >= T)) {           // есть левый сосед
						y->element[1] = y->element[0];
						y->ptr[2] = y->ptr[1];
						y->ptr[1] = y->ptr[0];
						y->n++;
						y->element[0] = x->element[i];
						x->element[i] = z->element[z->n - 1];
						z->n--;
						InsertTable(y->element[0].key, y, tbl);
						InsertTable(x->element[i].key, x, tbl);
						y->ptr[0] = z->ptr[z->n + 1];
						//z->ptr[z->n + 1] = NULL;
					}
					else if ((y->n == (T - 1)) && (z1->n >= T)) {        // есть правый сосед
						y->element[1] = x->element[i];
						InsertTable(y->element[1].key, y, tbl);
						x->element[i] = z1->element[0];
						InsertTable(x->element[i].key, x, tbl);
						y->ptr[2] = z1->ptr[0];
						y->n++;
						z1->n--;
						//переместить элементы
						for (j = 0; j < z1->n; ++j)
							z1->element[j] = z1->element[j + 1];
						for (j = 0; j <= z1->n; ++j)
							z1->ptr[j] = z1->ptr[j + 1];
						//z1->ptr[j] = NULL;
					}
				}
					else if ((y->n == (T - 1)) && (z->n == (T - 1))) {
						z->element[1] = x->element[i];
						z->n++;
						x->n--;
						InsertTable(z->element[1].key, z, tbl);
						for (j = i; j < x->n; ++j)
						{
							x->element[j] = x->element[j + 1];
							x->ptr[j + 1] = x->ptr[j + 2];
						}
						//x->ptr[j + 1] = NULL;
						z->n++;
						z->element[2] = y->element[0];
						InsertTable(z->element[2].key, z, tbl);
						z->ptr[2] = y->ptr[0];
						z->ptr[3] = y->ptr[1];
						free(y);
						y = z;
					}
				
			}
			x = y;
		}
		else {
			y = x->ptr[i];
			z = x ->ptr[i + 1];
			if (y->n >= T) {
				q = FindMax(y, &h);
				x->element[i].key = q->element[h].key;
				x->element[i].info = _strdup(q->element[h].info);
				x = y;
				fl=DelItem(&x, q->element[h].key, tbl);
				return fl;
			}
			else if (z->n >= T) {
				q = FindMin(z, &h);
				x->element[i].key = q->element[h].key;
				x->element[i].info = _strdup(q->element[h].info);
				x = z;
				fl=DelItem(&x, q->element[h].key, tbl);
				return fl;
			}
			else {
				y->element[1] = x->element[i];
				y->n++;
				InsertTable(y->element[1].key, y, tbl);
				x->n--;
				for (j = i; j < x->n; ++j) {
					x->element[j] = x->element[j + 1];
					x->ptr[j + 1] = x->ptr[j + 2];
				}
				//x->ptr[j + 1] = NULL;
				y->element[2] = z->element[0];
				y->n++;
				InsertTable(y->element[2].key, y, tbl);
				free(z);
				x = y;
				fl=DelItem(&x, k, tbl);
				return fl ;
			}
		}
	}
	i = 0;
	while((i < x->n)&& (k > x->element[i].key))
		++i;
	if ((i < x->n) && (k == x->element[i].key))
	{
		free(x->element[i].info);
		x->n--;
		for (j = i; j < x->n; ++j)
			x->element[j] = x->element[j + 1];
		return 1;
	}
	return 0;                     // элемент не найден			
}
void PrintTree(Node* ptr, int lvl) {
	int i,j;
	if (!DefineLeaf(ptr)) {
		for (i = ptr->n; i > 0; --i)
		{
			for (j = 0; j<lvl; ++j)
				printf("...");
			printf("%d\n", ptr->element[i-1].key);
		}
		return;
	}
	for (i = ptr->n; i > 0; --i)
	{
		PrintTree(ptr->ptr[i], lvl+1);
		for(j=0;j<lvl;++j)
			printf("...");
		printf("%d\n", ptr->element[i-1].key);
	}
	PrintTree(ptr->ptr[i], lvl + 1);
	return;
}
void TreeOut(Node*x) {
	int i;
	if (!DefineLeaf(x)) {
		for (i = 0; i < x->n; ++i)
			printf("Ключ:%d-Информация:\"%s\"\n", x->element[i].key, x->element[i].info);
		return;
	}
	for (i = 0; i < x->n; ++i) {
		TreeOut(x->ptr[i]);
		printf("Ключ:%d-Информация:\"%s\"\n", x->element[i].key, x->element[i].info);
	}
	TreeOut(x->ptr[i]);
	return;
}
void DelTree(Node* x) {
	int i;
	if (!DefineLeaf(x))
	{
		for (i = 0; i < x->n; ++i)
			free(x->element[i].info);
		return;
	}
	for (i = 0; i < x->n; ++i) {
		DelTree(x->ptr[i]);
		free(x->element[i].info);
	}
	DelTree(x->ptr[i]);
	free(x);
}
Node* Find(Node* x, int k, Table* tbl, int*a) {
	Node* pp = NULL;
	int i=0;
	if ((tbl[k%N].key == k)&&(tbl[k%N].p != NULL)) {
		pp = tbl[k%N].p;
		while ((i < pp->n) && (k != pp->element[i].key))
			++i;
		*a = i;
		return pp;
	}
	while (DefineLeaf(x)) {
		while ((i < x->n) && (k > x->element[i].key))
			++i;
		if (i == 3)
			x = x->ptr[i];
		else if (k != x->element[i].key)
			x = x->ptr[i];
		else {
			*a = i;
			pp = x;
			break;
		}
		i = 0;
	}
		while ((i < x->n) && (k > x->element[i].key))
			++i;
		if (k == x->element[i].key) {
			*a = i;
			pp = x;
		}
	
		return pp;
}
int getInt(int*pa)		 // ввод числа
{
	int n;
	do
	{
		n = scanf_s("%d", pa);
		if (n == 0)
		{
			printf("Error!\n");
			scanf_s("%*c");
		}
	} while (n == 0);
	return n < 0 ? 0 : 1;
}
char *getstr()			// ввод строки
{
	char *ptr = (char *)malloc(1);
	char buf[81];
	int n, len = 0;
	scanf("%*c");
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
			scanf("%*c");
		else {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} while (n > 0);
	return ptr;
}
int Add (Node**r, Table *tbl)        // диалоговая функция добавления элемента в дерево
{
	int k,g;
	char*s = NULL;
	printf("Введите ключ \n");                       // ввод информации
	getInt(&k);
	printf("Введите информацию\n");
	s = getstr();
	g = InsertTree(r, tbl, k, s);                                  // попытка добавления элемента в дерево, с последующим анализом возможных ошибок
	if (g == 1)
		printf("Элемент с таким ключом уже есть в  дереве\n");
	if (g == 0)
		printf("Элемент успешно добавлен в дерево\n");
	free(s);
	return 1;
}
int Delete(Node**r, Table *tbl) {
	int k, d;
	printf("Введите ключ\n");
	getInt(&k);
	d = DelItem(r, k, tbl);
	if (d == 0)
		printf("Такого эелемента нет в дереве\n");
	if (d == 1) {
		printf("Элемент успешно удален\n");
		DelTabItem(tbl, k);
	}
	return 1;
}
int Show(Node** r, Table *tbl)
{
	PrintTree(*r, 0);
	return 1;
}
int Print(Node**r, Table *tbl)
{
	TreeOut(*r);
	return 1;
}
int Search(Node** r, Table *tbl)
{
	Node*ptr = NULL;
	int a, k;
	printf("Введите ключ\n");
	getInt(&k);
	ptr = Find(*r, k, tbl, &a);
	if (ptr == NULL)
		printf("Элемента с таким ключом нет в дереве\n");
	else
		printf("Ключ:%d-Информация:\"%s\"\n", ptr->element[a].key, ptr->element[a].info);
	return 1;
}
int Max(Node **r, Table *tbl)
{
	Node*ptr = NULL;
	int a;
	ptr = FindMax(*r, &a);
	if (a == -1)
	{
		printf("Дерево пусто\n");
			return 1;
	}
	printf("Ключ:%d-Информация:\"%s\"\n", ptr->element[a].key, ptr->element[a].info);
	return 1;
}
int End(Node**r, Table *tbl) {
	DelTree(*r);
	return 1;
}
int AddRand(Node**r, Table *tbl)        // диалоговая функция добавления элемента в дерево
{
	int k, g, i, j=0;
	char *s = NULL;
	srand(time(NULL));
	while (j < 10000) { 
		     // ввод информации
		k = rand();
		char *s = (char*)malloc(10 * sizeof(char));
		for (i = 0; i < 9; ++i)
			s[i] = rand() % ('a' - 'z') + 'a';
		s[9] = '\0';
		g = InsertTree(r, tbl, k, s); 
		if (g == 1)
			printf("Элемент с таким ключом уже есть в  дереве\n");
		if (g == 0) {
			printf("Элемент успешно добавлен в дерево\n");
			j++;
		}
		free(s);
	}
	return 1;
}
int AddFile(Node**r, Table*tbl) {
	FILE * fd = fopen("Tree.txt", "r");
	int k, g, n;
	char s[10];
	if (fd == NULL) {
		puts("Файл не открыт");
			return 0;
	}
	fseek(fd, 0, SEEK_SET);
	while (!feof(fd)) {
		n=fscanf(fd, "%d", &k);
		n=fscanf(fd,"%s",s);
		if (n == (- 1))
			continue;
		g = InsertTree(r, tbl, k, s);
		if (g == 1)
			printf("Элемент с таким ключом уже есть в  дереве\n");
		if (g == 0) 
			printf("Элемент успешно добавлен в таблицу\n");
	}
	fclose(fd);
	return 1;
}
int Dialog(char*msg[], int n)                                                 // вывод альтернативных действий и получение кода действия от пользователя
{
	int i, r;
	for (i = 0; i < n; ++i)                                                   // вывод альтернатив
		printf("%s\n", msg[i]);
	getInt(&r);                                                                // ввод кода действия
	while ((r < 0) || (r >= n)) {                                              // проверка на правильность ввода кода действия
		printf("Введено неверное число, повторите ввод\n");
		getInt(&r);
	}
	return r;
}
int main ()
{
	char*msg[] = { "0. Выход", "1. Добавить элемент", "2. Удалить элемент", "3. Найти элемент", "4. Вывести дерево на экран","5. Найти макисмальный ключ", "6. Форматированный вывод", "7. Рандомное заполнение","8. Чтение дерева из файла" }; // альтернативные действия
	int Nmsg = sizeof(msg) / sizeof(msg[0]);    // количество действий
	int rc,i;
	Node p;
	Node*root = &p;
	Table tb [N];
	root->n = 0;
	for (i = 0; i < 4; ++i)
	   root->ptr[i] = NULL;
	for (i = 0; i < N; ++i)
		tb[i].p = NULL;
	setlocale(LC_ALL, "Rus");
	int(*fptr[])(Node**,Table*) = { End,Add,Delete,Search,Print,Max,Show, AddRand, AddFile };    // массив указателей на диалоговые функции
	while (rc = Dialog(msg, Nmsg))
		fptr[rc](&root,tb);
	End(&root,tb);
	return 0;
}
int timemain() {
	clock_t st, ls;
	int rc, i,a;
	Node p;
	Node*root = &p;
	Table tb[N];
	int d[10000];
	root->n = 0;
	srand(time(NULL));
	for (i = 0; i < 4; ++i)
		root->ptr[i] = NULL;
	for (i = 0; i < N; ++i)
		tb[i].p = NULL;
	for (i = 0; i < 10000; ++i)
		d[i] = rand() % (2 * N);
	setlocale(LC_ALL, "Rus");
	AddRand(&root, tb);
	st = clock();
	for (i = 0; i < 10000; ++i)
		Find(root, d[i], tb, &a);
	ls = clock();
	End(&root, tb);
	printf("%d\n", ls - st);
	return 0;	
}
