#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

const int maxtablesize = 6;
const char *msgs[] = { "   0. Выход", "   1. Добавить элемент", "   2. Показать таблицу", "   3. Найти элемент по заданному ключу", "   4. Удалить элемент по заданному ключу" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

typedef struct data {
	int key1;
	int key2;
	char *string;
} data;

typedef struct table {
	int tablesize;
	data* info;
	data* masukaz[maxtablesize];
} table;

int* getInt(int*);
int* getIntKey(int*);
int enterdata(table*);
int printtable(table*);
int insertelem(table*, int*, int*, char*);
int poiskpovtora(table *, int*, int*);
int dialog();
int findelem(table*);
int poiskelem(table*, int*, data*);
int poiskelem1(table*, int*, data*);
void docopy(table*, data*, int);
void docopy1(table*, data*, int);
int deleteelem(table* ukaz);
int udalelem(table* ukaz, int*);

int(*choice[])(table*) = { NULL, enterdata, printtable, findelem, deleteelem };

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	table table = { 0, (data *)calloc(maxtablesize, sizeof(data)) }; 
	int n;
	while (n = dialog()) {
		choice[n](&table);
	}
	return 0;
}

int* getInt(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr, 4); 
		if ((n == 0) || (*adr<0) || (*adr>(NMsgs-1)))
		{
			printf_s("   Ошибка. Повторите ввод \n   ");
			scanf_s("%*[^\n]");	
		}
	} while ((n == 0) || (*adr<0) || (*adr>(NMsgs-1)));
	scanf_s("%*c");
	return adr;
}

int* getIntKey(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr, 4); 
		if ((n == 0) || (*adr<=0))
		{
			printf_s("   Ошибка. Повторите ввод \n   ");
			scanf_s("%*[^\n]"); 
		}
	} while ((n == 0) || (*adr<=0));
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

int enterdata(table* ukaz) {
	int i = 0; data *ptr = (ukaz->info) + ukaz->tablesize;
	char *pchar = NULL;
	int flag;
	int k1;
	int k2;
	int* p1 = NULL;
	int* p2 = NULL;
	if ((ukaz->tablesize) < maxtablesize) {
		do {
			printf_s("   Введите ключ первого пространства\n   ");
			p1 = getIntKey(&k1); 
			printf_s("   Введите ключ второго пространства\n   ");
			p2 = getIntKey(&k2); 
			printf_s("   Введите данные\n   ");
			pchar = getstr();
			printf_s("\n");
			flag = insertelem(ukaz, p1, p2, pchar);
			p1 = 0; p2 = 0;
		} while (!flag);
	}
	else printf_s("   Таблица переполнена!\n\n");
	return 0;
}

int insertelem(table *ukaz, int *p1, int *p2, char* pchar) {
	data *ptr = (ukaz->info) + ukaz->tablesize; 
	int i=ukaz->tablesize-1;
	if ((*p1 == *p2) || poiskpovtora(ukaz, p1, p2)) {
		printf_s("   Ошибка. Дублирование ключей невозможно. Повторите попытку!\n   "); return 0;
	}
	else {
		if (!ukaz->tablesize) {
			ukaz->masukaz[0] = ukaz->info;
		}
			else {
				ukaz->masukaz[i+1] = ukaz->info + ukaz->tablesize;
				while ((i>=0) && ((ukaz->masukaz[i]->key2)>(*p2))) {
					ukaz->masukaz[i + 1] = ukaz->masukaz[i];
					i--;
					}
				ukaz->masukaz[i + 1] = ptr;
				}
			ptr->key1 = *p1;
			ptr->key2 = *p2;
			ptr->string = pchar;
			(ukaz->tablesize)++;
			ptr++;
		}
	return 1;
}

int printtable(table *ukaz) {
	int i = 0; int j = 0;
	data *ptr = ukaz->info;
	if (ukaz->tablesize == 0) printf_s("   Таблица пуста\n\n");
	else {
		printf_s("   Просматриваемая таблица:\n\n");
		while (i < ukaz->tablesize) {
			printf_s("   №%d, ключ 1: %d, ключ 2: %d, данные: \"%s\"\n", i, ptr->key1, ptr->key2, ptr->string);
			i++;
			ptr++;
		}
	printf_s("\n");
	printf_s("   Упорядоченная таблица:\n\n");
	data* ptr1 = ukaz->masukaz[0];
		while (j < ukaz->tablesize) {
			printf_s("   №%d, ключ 1: %d, ключ 2: %d, данные: \"%s\"\n", j, ukaz->masukaz[j]->key1, ukaz->masukaz[j]->key2, ukaz->masukaz[j]->string);
			j++;
		}
		printf_s("\n");
	}
	return 0;
}

int poiskpovtora(table *ukaz, int *key1, int* key2) {
	int i;
	data* ptr = ukaz->info;
		for (i = 0; i < ukaz->tablesize; i++, ptr++) {
			if ((ptr->key2 == *key1) || (ptr->key1 == *key1) || (ptr->key2 == *key2) || (ptr->key1 == *key2)) return 1;
		}  
	return 0;
}

int dialog() {
	int i; int k; int* choice;
	for (i = 0; i < NMsgs; i++) {
		printf_s("%s\n",msgs[i]);
	}
	printf_s("   Ваш выбор: "); 
	choice = getInt(&k);
	printf_s("\n");
	return *choice;
}

int findelem(table* ukaz) {
	int p; data* copy = (data *)calloc(1, sizeof(data)); 
	int index;
	int* key=NULL;
	if (!ukaz->tablesize) printf_s("   Таблица пуста!\n\n");
	else {
		printf_s("   Введите ключ, по которому нужно отыскать элемент\n   ");
		key = getIntKey(&p);
		if (index = poiskelem(ukaz, key, copy) != -1) {
			printf_s("   По запросу был найден элемент:\n");
			printf_s("   Ключ 1: %d, ключ 2: %d, данные: \"%s\"\n\n", copy->key1, copy->key2, copy->string);
		}
		else {
			if (index = poiskelem1(ukaz, key, copy) != -1) {
				printf_s("   По запросу был найден элемент:\n");
				printf_s("   Ключ 1: %d, ключ 2: %d, данные: \"%s\"\n\n", copy->key1, copy->key2, copy->string);
			}
			else printf_s("   Элемент по заданному ключу не найден\n\n");
		}
	}
	return 0;
}

int poiskelem(table *ukaz, int *key, data* copy) {
	int i = 0; int m = ukaz->tablesize - 1; int j;
	while ((m-i)>1) {
		j = (i+m) / 2;
		if ((ukaz->masukaz[j]->key2) == (*key)) {
			docopy(ukaz, copy, j);
			return j;
		}
		else
			{
			if ((ukaz->masukaz[j]->key2) > (*key))  m = j - 1;
			else { 
				if ((ukaz->masukaz[j]->key2) < (*key)) i = j + 1;
				}
			}
	}
	if ((ukaz->masukaz[i]->key2) == (*key)) {
		docopy(ukaz, copy, i);
			return i;
	}
	else {
		if ((ukaz->masukaz[m]->key2) == (*key)) {
			docopy(ukaz, copy, m);
			return m;
		}
	}
	return -1;
}

int poiskelem1(table* ukaz, int* key, data* copy) {
	data* ptr = ukaz->info; int i;
	for (i = 0; i < ukaz->tablesize; i++,ptr++) {
		if (ptr->key1 == *key) {
			docopy1(ukaz, copy, i);
			return i;
		}
	}
	return -1;
}

void docopy(table* ukaz, data* copy, int index) {
	copy->key1 = ukaz->masukaz[index]->key1;
	copy->key2 = ukaz->masukaz[index]->key2;
	copy->string = ukaz->masukaz[index]->string;
}

void docopy1(table* ukaz, data* copy, int index) {
	data* ptr = ukaz->info + index;
	copy->key1 = ptr->key1;
	copy->key2 = ptr->key2;
	copy->string = ptr->string;
}

int deleteelem(table* ukaz) {
	int p;
	int* key = NULL;
	int success;
	if (!ukaz->tablesize) printf_s("   Таблица пуста, в ней нечего удалять!\n\n");
	else {
		printf_s("   Введите ключ, по которому нужно удалить элемент\n   ");
		key = getIntKey(&p);
		if (!(success = udalelem(ukaz, key))) printf_s("   Элемент по заданному ключу не был найден\n\n");
		else printf_s("   Элемент был успешно удалён\n\n");
	}
	return 0;
}

int udalelem(table* ukaz, int* key) {
	data* copy = (data *)calloc(1, sizeof(data));
	int index, index1;
	int i, i1, ii;
	int k = 0;
	data* ptr1=ukaz->info; data* ptr2;
	data* ptr = ukaz->info;
	int n = ukaz->tablesize - 1; int m = n;
	index1 = poiskelem1(ukaz, key, copy);
	if (((index = poiskelem(ukaz, key, copy)) == -1) && (index1 == -1)) {
		free(copy);
		return 0;
	}
	else {
		if (index != -1) {
			k = ukaz->masukaz[index]->key1;
			i1 = poiskelem1(ukaz, &k, copy);
			while (n - index) {
				ukaz->masukaz[index] = ukaz->masukaz[index + 1];
				index++;
			}
			while (n - i1) {
				k = (ukaz->info+i1+1)->key2;
				ii = poiskelem(ukaz, &k, copy);
				ukaz->masukaz[ii] = ukaz->info + i1 + 1;
				ukaz->info[i1] = ukaz->info[i1 + 1];
				i1++;
			}
		}
		else {
			k = (ukaz->info + index1)->key2;
			i = poiskelem(ukaz, &k, copy);
			while (n - i) {
				ukaz->masukaz[i] = ukaz->masukaz[i + 1];
				i++;
			}
			while (n - index1) {
				ukaz->info[index1] = ukaz->info[index1 + 1];
				index1++;
			}
		}
	}
	free(copy);
	ukaz->tablesize--;
	return 1;
}
