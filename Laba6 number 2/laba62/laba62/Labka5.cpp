#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

//const int maxtablesize = 7;
const char *msgs[] = { "   0. Выход", "   1. Добавить элемент", "   2. Показать таблицу", "   3. Найти элемент по заданному ключу", "   4. Удалить элемент по заданному ключу" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

struct PK1 {
	int key1;
	long offset;
	long len;
};

struct PK2 {
	int key2;
	long offset;
	long len;
};

struct Table {
	int maxtablesize;
	int size;
	long keys1off;
	long keys2off;
	PK1* tab1;
	PK2* tab2;
	FILE* fd;
};

struct copy {
	int ckey1;
	int ckey2;
	char* cinfo;
	int offset1;
	int offset2;
};


int* getInt(int*);
int* getIntK(int*);
int* getIntKey(int*);
int enterdata(Table*);
int printtable(Table*);
int insertelem(Table*, int*, int*, char*);
int poiskpovtora(Table *, int*, int*);
int dialog();
int findelem(Table*);
int poiskelem(Table*, int*, copy*, int*, int*);
int poiskelem1(Table*, int*, copy*, int*, int*);
int deleteelem(Table* ukaz);
int udalelem(Table* ukaz, int*, int*);
void udalil(Table* ukaz, int*, int*);
void init(Table*);
void erase(Table*);
FILE* openfile(Table*);
FILE* closefile(Table*);
FILE* opentablefile(Table*);

int(*choice[])(Table*) = { NULL, enterdata, printtable, findelem, deleteelem };

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Table Table;
	int n;
	 Table.fd = openfile(&Table);
	while (n = dialog()) {
		choice[n](&Table);
	}
	closefile(&Table);
	erase(&Table);
	fclose(Table.fd);
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

int* getIntK(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr);
		if ((n == 0) || (*adr<=0) || (*adr>2))
		{
			printf_s("   Ошибка. Повторите ввод \n   ");
			scanf_s("%*[^\n]");
			scanf_s("%*c");
		}
	} while ((n == 0) || (*adr<=0) || (*adr>2));
	
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

int enterdata(Table* ukaz) {
	int i = 0;
	char *pchar = NULL;
	int flag;
	int k1;
	int k2;
	int* p1 = NULL;
	int* p2 = NULL;
	if ((ukaz->size) < ukaz->maxtablesize) {
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
			if (!flag) printf_s("   Ошибка. Дублирование ключей невозможно. Повторите попытку!\n");
		} while (!flag);
	}
	else printf_s("   Таблица переполнена!\n\n");
	return 0;
}

int insertelem(Table *ukaz, int *p1, int *p2, char* pchar) {
	PK1* ptr1 = ukaz->tab1 + ukaz->size;
	PK2* ptr2 = ukaz->tab2;
	int i = ukaz->size - 1;
	if (poiskpovtora(ukaz, p1, p2)) {                                
		 return 0;
	}
	else {
		if (!ukaz->size) {
			ptr1 = ukaz->tab1;
			ptr2 = ukaz->tab2;
			ptr1->key1 = *p1;
			ptr2->key2 = *p2;
			ptr1->len = strlen(pchar) + 1;
			ptr2->len = ptr1->len;
			ptr1->offset = 3*sizeof(long);
			ptr2->offset = 3*sizeof(long);
			fseek(ukaz->fd, ptr1->offset, SEEK_SET);
			fwrite(pchar, sizeof(char), ptr1->len, ukaz->fd);

		}
		else {
			ptr1->key1 = *p1;
			ptr1->len = strlen(pchar) + 1;
			ptr1->offset = (ptr1 - 1)->offset + (ptr1 - 1)->len;
			fseek(ukaz->fd, ptr1->offset, SEEK_SET);
			fwrite(pchar, sizeof(char), ptr1->len, ukaz->fd);
			ukaz->keys1off = ftell(ukaz->fd);
			ptr1++;
			while ((i >= 0) && (((ptr2+i)->key2)>(*p2))) {
				ukaz->tab2[i + 1] = ukaz->tab2[i];
				i--;
			}
			ukaz->tab2[i + 1].key2 = *p2;
			ukaz->tab2[i + 1].len = (ptr1 - 1)->len;
			ukaz->tab2[i + 1].offset = (ptr1 - 1)->offset;
		}

		(ukaz->size)++;
	}
	return 1;
}

int printtable(Table *ukaz) {
	int i = 0;
	int j = 0;
	int index=0;
	int index1=0;
	int key;
	copy* copy1 = (copy*)calloc(1, sizeof(copy));
	PK1* ptr1 = ukaz->tab1;
	if (ukaz->size == 0) printf_s("   Таблица пуста\n\n");
	else {
		printf_s("   Просматриваемая таблица:\n\n");
		while (i < ukaz->size) {
			key = (ukaz->tab1 + i)->key1;
			index1 = poiskelem1(ukaz, &key, copy1, &index, &index1);
			printf_s("   №%d, ключ 1: %d, ключ 2: %d, данные: \"%s\"\n", i, copy1->ckey1, copy1->ckey2, copy1->cinfo);
			i++;
			ptr1++;
		}
		printf_s("\n");
		printf_s("   Упорядоченная таблица:\n\n");
		PK2* ptr = ukaz->tab2;
		while (j < ukaz->size) {
			key = (ukaz->tab2 + j)->key2;
			index = poiskelem(ukaz, &key, copy1, &index, &index1);
			printf_s("   №%d, ключ 1: %d, ключ 2: %d, данные: \"%s\"\n", j, copy1->ckey1, copy1->ckey2, copy1->cinfo);
			j++;
			ptr++;
		}
		printf_s("\n");
	}
	free(copy1);
	copy1 = NULL;
	return 0;
}

int poiskpovtora(Table *ukaz, int *key1, int* key2) {
	int i;
	PK1* ptr1 = ukaz->tab1;
	PK2* ptr2 = ukaz->tab2;
	for (i = 0; i < ukaz->size; i++, ptr1++, ptr2++) {
		if ((ptr1->key1 == *key1) || (ptr2->key2 == *key2)) return 1;
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
	int p; int m = 1; int n = 1;
	int index = -1; 
	int index1 = -1;
	int* key = NULL;
	copy* copy1 = (copy*)calloc(1, sizeof(copy));
	if (!ukaz->size) printf_s("   Таблица пуста!\n\n");
	else {
		printf_s("   Введите ключ, по которому нужно отыскать элемент\n   ");
		key = getIntKey(&p);
		if ((index1 = poiskelem1(ukaz, key, copy1, &index, &index1)) != -1) {
			printf_s("   По запросу в 1-ом пространстве ключей был найден элемент:\n");
			printf_s("   Ключ 1: %d, ключ 2: %d, данные: \"%s\"\n\n", copy1->ckey1, copy1->ckey2, copy1->cinfo);
		}
		else { printf_s("   В  1-ом пространстве ключей элемента с таким ключом нет\n"); m = 0; }
			if ((index = poiskelem(ukaz, key, copy1, &index, &index1)) != -1) {
				n = index;
				printf_s("   По запросу в 2-ом пространстве ключей был найден элемент:\n");
				printf_s("   Ключ 1: %d, ключ 2: %d, данные: \"%s\"\n\n", copy1->ckey1, copy1->ckey2, copy1->cinfo);
			}
			else { printf_s("   Во 2-ом пространстве ключей элемента с таким ключом нет\n\n"); n = 0; }
			if (!m && !n) printf_s("   Следовательно, элемент по заданному ключу не найден\n\n");
		}
	free(copy1);
	copy1 = NULL;
	return 0;
}

int poiskelem(Table *ukaz, int *key, copy* copy1, int *index, int* index1) {
	int i = 0; 
	int m = ukaz->size - 1;
	int j, g;
	copy1->ckey1 = 0;
	copy1->ckey2 = 0;
	PK2* ptr = ukaz->tab2;
	while ((m - i) >= 0) {
		j = (i + m) / 2;
		ptr=ukaz->tab2+j;
		if ((ptr->key2) == (*key)) {
			copy1->ckey2 = ptr->key2;
			copy1->offset2 = ptr->offset;
				PK1* ptr1 = ukaz->tab1;
				fseek(ukaz->fd, ptr1->offset, SEEK_SET);
				for (g = 0; g < ukaz->size; g++, ptr1++) {
					if ((ptr1->offset) != (copy1->offset2)) {
						continue;
					}
					else {
						fseek(ukaz->fd, ptr1->offset, SEEK_SET);
						char *buf = (char *)calloc(ptr->len, sizeof(char *));
						fread(buf, sizeof(char), ptr->len, ukaz->fd);
						copy1->ckey1 = ptr1->offset;
						*index1 = g;
						copy1->ckey1 = ptr1->key1;
						copy1->cinfo = _strdup(buf);
						return j;
					}
				}
			}
		else
		{
			if (ptr->key2 > (*key)) { m = j - 1; }
			else  { i = j + 1; }
		}
	}
	return -1;
}

int poiskelem1(Table* ukaz, int* key, copy* copy1, int* index, int* index1) { 
	PK1* ptr = ukaz->tab1; int i; int j;
	copy1->ckey1 = 0;
	copy1->ckey2 = 0;
	copy1->cinfo = NULL;
	for (i = 0; i < ukaz->size; i++, ptr++) {
		if (ptr->key1 == *key) {
			copy1->ckey1 = *key;
			copy1->offset1 = ptr->offset;
			fseek(ukaz->fd, ptr->offset, SEEK_SET);
			PK2* ptrr = ukaz->tab2;
			
			for (j = 0; j < ukaz->size; j++, ptrr++) {
				if (ptrr->offset != copy1->offset1) { continue; }
				else {
					copy1->offset2 = ptrr->offset;
					char *buf = (char *)calloc(ptr->len, sizeof(char *));
					fseek(ukaz->fd, ptr->offset, SEEK_SET);
					fread(buf, sizeof(char), ptr->len, ukaz->fd);
					copy1->cinfo = buf;
					copy1->ckey2 = ptrr->key2;
					*index = j;
					return i;
				}
			}
		}
	}
	return -1;
}

int deleteelem(Table* ukaz) {
	int p; int p1;
	int* key = NULL; int* number;
	int success;
	if (!ukaz->size) printf_s("   Таблица пуста, в ней нечего удалять!\n\n");
	else {
		printf_s("   Введите ключ, по которому нужно удалить элемент\n   ");
		key = getIntKey(&p);
		printf_s("   Введите номер пространства ключей\n   ");
		number = getIntK(&p1);
		if (!(success = udalelem(ukaz, key, number))) printf_s("   Элемент по заданному ключу не был найден\n\n");
		else printf_s("   Элемент был успешно удалён\n\n");
	}
	return 0;
}

int udalelem(Table* ukaz, int* key, int* number) {
	copy* copy1 = (copy*)calloc(1, sizeof(copy));
	int index; int index1;
	if (*number == 2) {
		if ((index = poiskelem(ukaz, key, copy1, &index, &index1)) == -1) return 0;
		else
			udalil(ukaz, &index, &index1);
	}
	if (*number == 1) {
		if ((index1 = poiskelem1(ukaz, key, copy1, &index, &index1)) == -1) return 0;
		else
			udalil(ukaz, &index, &index1);
	}
	free(copy1);
	copy1 = NULL;
	return 1;
}

void udalil(Table* ukaz, int* index, int* index1) {
	int n1 = *index1; int n = *index;
	while (n < (ukaz->size - 1)) {
		ukaz->tab2[n] = ukaz->tab2[n + 1];
		n++;
	}
	while (n1 < (ukaz->size - 1)) {
		ukaz->tab1[n1] = ukaz->tab1[n1 + 1];
		n1++;
	}
	ukaz->size--;
	}

	void init(Table* ukaz) {
		ukaz->size = 0;
		ukaz->tab1 = (PK1*)calloc(ukaz->maxtablesize, sizeof(PK1));
		ukaz->tab2 = (PK2*)calloc(ukaz->maxtablesize, sizeof(PK2));
	}

	void erase(Table*ukaz) {
		free(ukaz->tab1);
		free(ukaz->tab2);
		ukaz->tab1 = NULL;
		ukaz->tab2 = NULL;
	}

	FILE* openfile(Table* ukaz) {
		char* filename; 
		FILE* fd1;
			printf_s("   Введите имя файла, который вы хотите открыть: ");
			filename = getstr();
			printf_s("\n");
			if (!(fopen_s(&(ukaz->fd), filename, "r+b"))) {
				opentablefile(ukaz);
			}
			else {
				printf_s("   Файл с таким именем не найден. Будет создан новый файл\n\n");
				fopen_s(&fd1, filename, "w+b");
				ukaz->fd = fd1;
				printf_s("   Введите максимальный размер таблицы: ");
				getIntKey(&ukaz->maxtablesize);
				init(ukaz);
				printf_s("\n\n");
			}
		return ukaz->fd;
	}

	FILE* closefile(Table* ukaz) {
		int chislo;
		rewind(ukaz->fd);
		chislo = ukaz->size;
		fwrite(&chislo, sizeof(long), 1, ukaz->fd);
		fseek(ukaz->fd, ukaz->keys1off, SEEK_SET);
		ukaz->keys1off = ftell(ukaz->fd);
		fseek(ukaz->fd, sizeof(long), SEEK_SET);
		chislo = ukaz->keys1off;
		fwrite(&chislo, sizeof(long), 1, ukaz->fd);
		int i;
		fseek(ukaz->fd, ukaz->keys1off, SEEK_SET);
		for (i = 0; i < ukaz->size; i++) {
			chislo = ukaz->tab1[i].key1;
			fwrite(&chislo, sizeof(long), 1, ukaz->fd);
			chislo = ukaz->tab1[i].offset;
			fwrite(&chislo, sizeof(long), 1, ukaz->fd);
			chislo = ukaz->tab1[i].len;
			fwrite(&chislo, sizeof(long), 1, ukaz->fd);
		}
		ukaz->keys2off = ftell(ukaz->fd);
		for (i = 0; i < ukaz->size; i++) {
			chislo = ukaz->tab2[i].key2;
			fwrite(&chislo, sizeof(long), 1, ukaz->fd);
			chislo = ukaz->tab2[i].offset;
			fwrite(&chislo, sizeof(long), 1, ukaz->fd);
			chislo = ukaz->tab2[i].len;
			fwrite(&chislo, sizeof(long), 1, ukaz->fd);
		}
		rewind(ukaz->fd);
		fseek(ukaz->fd, 2 * sizeof(long), SEEK_SET);
		chislo = ukaz->keys2off;
		fwrite(&chislo, sizeof(long), 1, ukaz->fd);
		chislo = ukaz->maxtablesize;
		fseek(ukaz->fd, 0, SEEK_END);
		fwrite(&chislo, sizeof(long), 1, ukaz->fd);
		return ukaz->fd;
	}

	FILE* opentablefile(Table* ukaz) {
		int i; int chislo[3];
		long pos = (-4);
		fseek(ukaz->fd, pos, SEEK_END);
		fread(chislo, sizeof(long), 1, ukaz->fd);
		ukaz->maxtablesize = chislo[0];
		init(ukaz);
		rewind(ukaz->fd);
		fread(&(ukaz->size), sizeof(int), 1, ukaz->fd);
		fread(&(ukaz->keys1off), sizeof(int), 1, ukaz->fd);
		fread(&(ukaz->keys2off), sizeof(int), 1, ukaz->fd); 
		for (i = 0; i < ukaz->size; i++) {
			fseek(ukaz->fd, (ukaz->keys1off + 3 * i * sizeof(long)), SEEK_SET);
			fread(chislo, sizeof(long), 3, ukaz->fd);
			ukaz->tab1[i].key1 = chislo[0];
			ukaz->tab1[i].offset = chislo[1];
			ukaz->tab1[i].len = chislo[2];
		}
		for (i = 0; i < ukaz->size; i++) {
			fseek(ukaz->fd, (ukaz->keys2off + 3 * i * sizeof(long)), SEEK_SET);
			fread(chislo, sizeof(long), 3, ukaz->fd);
			ukaz->tab2[i].key2 = chislo[0];
			ukaz->tab2[i].offset = chislo[1];
			ukaz->tab2[i].len = chislo[2];
		}
		
		printf_s("%d %d %d %d \n", ukaz->maxtablesize, ukaz->size, ukaz->keys1off, ukaz->keys2off);
		return ukaz->fd;
	}