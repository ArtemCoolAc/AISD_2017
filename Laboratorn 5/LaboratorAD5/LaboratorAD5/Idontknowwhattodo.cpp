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

struct PK1 {
	int key1;
	char *info;
};

struct PK2 {
	int key2;
	char *info;
};

struct Table {
	int size;
	PK1 tab1[maxtablesize];
	PK2 tab2[maxtablesize];
};

struct copy {
	int ckey1;
	int ckey2;
	char* cinfo;
};



int* getInt(int*);
int* getIntKey(int*);
int enterdata(Table*);
int printtable(Table*);
int insertelem(Table*, int*, int*, char*);
int poiskpovtora(Table *, int*, int*);
int dialog();
int findelem(Table*);
int poiskelem(Table*, int*, copy*, int*, int*);
int poiskelem1(Table*, int*, copy*, int*, int*);
void docopy(Table*, copy*, int);
void docopy1(Table*, copy*, int);
int deleteelem(Table* ukaz);
int udalelem(Table* ukaz, int*);

int(*choice[])(Table*) = { NULL, enterdata, printtable, findelem, deleteelem };

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Table Table = { 0, (int)calloc(maxtablesize, sizeof(int)), (char*)calloc(maxtablesize, sizeof(char*)) };
	int n;
	while (n = dialog()) {
		choice[n](&Table);
	}
	return 0;
}

int* getInt(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr, 4);
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

int enterdata(Table* ukaz) {
	int i = 0;
	char *ptr1 = (ukaz->tab1->info) + ukaz->size;
	char *ptr2 = (ukaz->tab2->info) + ukaz->size;
	char *pchar = NULL;
	int flag;
	int k1;
	int k2;
	int* p1 = NULL;
	int* p2 = NULL;
	if ((ukaz->size) < maxtablesize) {
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

int insertelem(Table *ukaz, int *p1, int *p2, char* pchar) {
	PK1* ptr1 = ukaz->tab1+ukaz->size;
	PK2* ptr2 = ukaz->tab2;
	int i = ukaz->size - 1;
	if ((*p1 == *p2) || poiskpovtora(ukaz, p1, p2)) {
		printf_s("   Ошибка. Дублирование ключей невозможно. Повторите попытку!\n   "); return 0;
	}
	else {
		if (!ukaz->size) {
			ptr1 = ukaz->tab1;
			ptr2 = ukaz->tab2;
			ptr1->key1 = *p1;
			ptr2->key2 = *p2;
			ptr1->info = pchar;
			ptr2->info = pchar;
		}
		else {
			ptr1->key1 = *p1;
			ptr1->info = pchar;
			ptr1++;
			while ((i >= 0) && ((ptr2->key2)>(*p2))) {
				ptr2++;
				i--;
			}
			(++ptr2)->key2 = *p2;
			ptr2->info = pchar;
		}
		
		(ukaz->size)++;
	}
	return 1;
}

int printtable(Table *ukaz) {
	int i = 0; int j = 0;
	if (ukaz->size == 0) printf_s("   Таблица пуста\n\n");
	
	else {
		printf_s("   Просматриваемая таблица:\n\n");
		while (i < ukaz->size) {
			printf_s("   №%d, ключ 1: %d, ключ 2: %d, данные: \"%s\"\n", i, ukaz->tab1[i], ukaz->tab2[i], ukaz->tab1->info);
			i++;
			ptr++;
		}
		printf_s("\n");
		printf_s("   Упорядоченная таблица:\n\n");
		data* ptr1 = ukaz->masukaz[0];
		while (j < ukaz->size) {
			printf_s("   №%d, ключ 1: %d, ключ 2: %d, данные: \"%s\"\n", j, ukaz->masukaz[j]->key1, ukaz->masukaz[j]->key2, ukaz->masukaz[j]->string);
			j++;
		}
		printf_s("\n");
	}
	return 0;
}

int poiskpovtora(Table *ukaz, int *key1, int* key2) {
	int i;
	PK1* ptr1 = ukaz->tab1;
	PK2* ptr2 = ukaz->tab2;
	for (i = 0; i < ukaz->size; i++, ptr1++, ptr2++) {
		if ((ptr2->key2 == *key1) || (ptr1->key1 == *key1) || (ptr2->key2 == *key2) || (ptr1->key1 == *key2)) return 1;
	}
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

int findelem(Table* ukaz) {
	int p;
	int* index; int* index1; *index = -1; *index1 = -1;
	
		int* key = NULL;
		copy* copy1 = (copy*) calloc(1, sizeof(copy)); 
	if (!ukaz->size) printf_s("   Таблица пуста!\n\n");
	else {
		printf_s("   Введите ключ, по которому нужно отыскать элемент\n   ");
		key = getIntKey(&p);
		if ((*index = poiskelem(ukaz, key, copy1, index, index1)) != -1) {
			printf_s("   По запросу был найден элемент:\n");
			printf_s("   Ключ 1: %d, ключ 2: %d, данные: \"%s\"\n\n", copy1->ckey1, copy1->ckey2, copy1->cinfo);
		}
		else {
			if ((*index1 = poiskelem1(ukaz, key, copy1, index, index1)) != -1) {
				printf_s("   По запросу был найден элемент:\n");
				printf_s("   Ключ 1: %d, ключ 2: %d, данные: \"%s\"\n\n", copy1->ckey1, copy1->ckey2, copy1->cinfo);
			}
		else printf_s("   Элемент по заданному ключу не найден\n\n");
	}
	return 0;
}

int poiskelem(Table *ukaz, int *key, copy* copy1, int *index, int* index1) {
	int i = 0; int m = ukaz->size - 1; int j; int g;
	PK2* ptr = ukaz->tab2;
	while ((m - i) > 0) {
		j = (i + m) / 2;
		ptr += j;
		if ((ptr->key2) == (*key)) {
			copy1->ckey2 = ptr->key2;
			copy1->cinfo = ptr->info;
			if (copy1->ckey1) int index = poiskelem1(ukaz, key, copy1);
			return j;
			PK1* ptr1 = ukaz->tab1;
			for (g = 0; g < ukaz->size; g++, ptr1++) {
				if ((ptr1->info) == (copy1->cinfo)) {
					*index1 = g;
					copy1->ckey1 = ptr1->key1;
					continue;
				}
			}
		}
		else
		{
			if (ptr->key2 > (*key))  m = j - 1;
			else  i = j + 1;
		}
		ptr -= j;
	}
	/* if ((ukaz->tab2+i)->key2 == (*key)) {
		docopy(ukaz, copy1, i);
		return i;
	}
	else {
		if (((ukaz->tab2+m)->key2) == (*key)) {
			docopy(ukaz, copy1, m);
			return m;
		}
	} */
	return -1;
}

int poiskelem1(Table* ukaz, int* key, copy* copy, int* index, int* index1) {
	PK1* ptr = ukaz->tab1; int i; char* stroka; int j;
	for (i = 0; i < ukaz->size; i++, ptr++) {
		if (ptr->key1 == *key) {
			copy->ckey1 = *key;
			stroka = (ukaz->tab1 + i)->info;
			copy->cinfo = stroka;
			PK2* ptrr = ukaz->tab2;
			for (j = 0; j < ukaz->size; j++, ptrr++) {
				if (ptrr->info == stroka) {
					copy->ckey2 = ptrr->key2;
					*index = j;
					continue;
				}
			}
			*index1 = i;
			return i;
		
		}
	}
	return -1;
}

int deleteelem(Table* ukaz) {
	int p;
	int* key = NULL;
	int success;
	if (!ukaz->size) printf_s("   Таблица пуста, в ней нечего удалять!\n\n");
	else {
		printf_s("   Введите ключ, по которому нужно удалить элемент\n   ");
		key = getIntKey(&p);
		if (!(success = udalelem(ukaz, key))) printf_s("   Элемент по заданному ключу не был найден\n\n");
		else printf_s("   Элемент был успешно удалён\n\n");
	}
	return 0;
}

