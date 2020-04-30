#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

const int maxtablesize = 6;
const char *msgs[] = { "   0. �����", "   1. �������� �������", "   2. �������� �������", "   3. ����� ������� �� ��������� �����", "   4. ������� ������� �� ��������� �����" };
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
	int size;
	long keys1off;
	long keys2off;
	PK1 tab1[maxtablesize];
	PK2 tab2[maxtablesize];
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
int enterdata(Table*, FILE*);
int printtable(Table*, FILE*);
int insertelem(Table*, int*, int*, char*, FILE*);
int poiskpovtora(Table *, int*, int*);
int dialog();
int findelem(Table*, FILE*);
int poiskelem(Table*, int*, copy*, int*, int*, FILE*);
int poiskelem1(Table*, int*, copy*, int*, int*, FILE*);
int deleteelem(Table* ukaz, FILE*);
int udalelem(Table* ukaz, int*, int*, FILE*);
void udalil(Table* ukaz, int*, int*, FILE*);
void init(Table*);
void erase(Table*);
FILE* openfile(Table*);
FILE* closefile(Table*, FILE*);

int(*choice[])(Table*, FILE*) = { NULL, enterdata, printtable, findelem, deleteelem };

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Table Table;
	init(&Table);
	int n;
	FILE* fd = openfile(&Table);
	while (n = dialog()) {
		choice[n](&Table, fd);
	}
	erase(&Table);
	closefile(&Table, fd);
	fclose(fd);
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
			printf_s("   ������. ��������� ���� \n   ");
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
			printf_s("   ������. ��������� ���� \n   ");
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
			printf_s("   ������. ��������� ���� \n   ");
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
		if (n < 0) { // ����� ������ ����� ������ ��� �����
			free(ptr); // ����������� ������
			ptr = NULL;
			continue;
		}
		if (n == 0)
			scanf_s("%*c");
		else {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 1);
			strcat(ptr, buf); //�� ��������� � ������ ���
		}
	} while (n > 0);
	return ptr;
}

int enterdata(Table* ukaz, FILE* fd) {
	int i = 0;
	char *pchar = NULL;
	int flag;
	int k1;
	int k2;
	int* p1 = NULL;
	int* p2 = NULL;
	if ((ukaz->size) < maxtablesize) {
		do {
			printf_s("   ������� ���� ������� ������������\n   ");
			p1 = getIntKey(&k1);
			printf_s("   ������� ���� ������� ������������\n   ");
			p2 = getIntKey(&k2);
			printf_s("   ������� ������\n   ");
			pchar = getstr();
			printf_s("\n");
			flag = insertelem(ukaz, p1, p2, pchar,fd);
			p1 = 0; p2 = 0;
			if (!flag) printf_s("   ������. ������������ ������ ����������. ��������� �������!\n");
		} while (!flag);
	}
	else printf_s("   ������� �����������!\n\n");
	return 0;
}

int insertelem(Table *ukaz, int *p1, int *p2, char* pchar, FILE* fd) {
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
			fseek(fd, ptr1->offset, SEEK_SET);
			fwrite(pchar, sizeof(char), ptr1->len, fd);
		}
		else {
			ptr1->key1 = *p1;
			ptr1->len = strlen(pchar) + 1;
			ptr1->offset = (ptr1 - 1)->offset + (ptr1 - 1)->len;
			fseek(fd, ptr1->offset, SEEK_SET);
			fwrite(pchar, sizeof(char), ptr1->len, fd);
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

int printtable(Table *ukaz, FILE* fd) {
	int i = 0;
	int j = 0;
	int index=0;
	int index1=0;
	int key;
	copy* copy1 = (copy*)calloc(1, sizeof(copy));
	PK1* ptr1 = ukaz->tab1;
	if (ukaz->size == 0) printf_s("   ������� �����\n\n");
	else {
		printf_s("   ��������������� �������:\n\n");
		while (i < ukaz->size) {
			key = (ukaz->tab1 + i)->key1;
			index1 = poiskelem1(ukaz, &key, copy1, &index, &index1, fd);
			printf_s("   �%d, ���� 1: %d, ���� 2: %d, ������: \"%s\"\n", i, copy1->ckey1, copy1->ckey2, copy1->cinfo);
			i++;
			ptr1++;
		}
		printf_s("\n");
		printf_s("   ������������� �������:\n\n");
		PK2* ptr = ukaz->tab2;
		while (j < ukaz->size) {
			key = (ukaz->tab2 + j)->key2;
			index = poiskelem(ukaz, &key, copy1, &index, &index1, fd);
			printf_s("   �%d, ���� 1: %d, ���� 2: %d, ������: \"%s\"\n", j, copy1->ckey1, copy1->ckey2, copy1->cinfo);
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
	printf_s("   ��� �����: ");
	choice = getInt(&k);
	printf_s("\n");
	return *choice;
}

int findelem(Table* ukaz, FILE* fd) {
	int p; int m = 1; int n = 1;
	int index = -1; 
	int index1 = -1;
	int* key = NULL;
	copy* copy1 = (copy*)calloc(1, sizeof(copy));
	if (!ukaz->size) printf_s("   ������� �����!\n\n");
	else {
		printf_s("   ������� ����, �� �������� ����� �������� �������\n   ");
		key = getIntKey(&p);
		if ((index1 = poiskelem1(ukaz, key, copy1, &index, &index1, fd)) != -1) {
			printf_s("   �� ������� � 1-�� ������������ ������ ��� ������ �������:\n");
			printf_s("   ���� 1: %d, ���� 2: %d, ������: \"%s\"\n\n", copy1->ckey1, copy1->ckey2, copy1->cinfo);
		}
		else { printf_s("   �  1-�� ������������ ������ �������� � ����� ������ ���\n"); m = 0; }
			if ((index = poiskelem(ukaz, key, copy1, &index, &index1, fd)) != -1) {
				n = index;
				printf_s("   �� ������� � 2-�� ������������ ������ ��� ������ �������:\n");
				printf_s("   ���� 1: %d, ���� 2: %d, ������: \"%s\"\n\n", copy1->ckey1, copy1->ckey2, copy1->cinfo);
			}
			else { printf_s("   �� 2-�� ������������ ������ �������� � ����� ������ ���\n\n"); n = 0; }
			if (!m && !n) printf_s("   �������������, ������� �� ��������� ����� �� ������\n\n");
		}
	free(copy1);
	copy1 = NULL;
	return 0;
}

int poiskelem(Table *ukaz, int *key, copy* copy1, int *index, int* index1, FILE* fd) {
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
				for (g = 0; g < ukaz->size; g++, ptr1++) {
					
					fseek(fd, ptr1->offset, SEEK_SET);
					if ((ptr1->offset) != (copy1->offset2)) {
						continue;
					}
					else {
						fseek(fd, ptr->offset, SEEK_SET);
						char *buf = (char *)calloc(ptr->len, sizeof(char *));
						fread(buf, sizeof(char), ptr->len, fd);
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

int poiskelem1(Table* ukaz, int* key, copy* copy1, int* index, int* index1, FILE* fd) { 
	PK1* ptr = ukaz->tab1; int i; char* stroka; int j;
	copy1->ckey1 = 0;
	copy1->ckey2 = 0;
	copy1->cinfo = NULL;
	for (i = 0; i < ukaz->size; i++, ptr++) {
		if (ptr->key1 == *key) {
			copy1->ckey1 = *key;
			copy1->offset1 = ptr->offset;
			fseek(fd, ptr->offset, SEEK_SET);
			PK2* ptrr = ukaz->tab2;
			for (j = 0; j < ukaz->size; j++, ptrr++) {
				if (ptrr->offset != copy1->offset1) { continue; }
				else {
					copy1->offset2 = ptrr->offset;
					char *buf = (char *)calloc(ptr->len, sizeof(char *));
					fread(buf, sizeof(char), ptr->len, fd);
					copy1->cinfo = _strdup(buf);
					*index = j;
					copy1->ckey2 = ptrr->key2;
					return *index;
				}
			}
		}
	}
	return -1;
}

int deleteelem(Table* ukaz, FILE* fd) {
	int p; int p1;
	int* key = NULL; int* number;
	int success;
	if (!ukaz->size) printf_s("   ������� �����, � ��� ������ �������!\n\n");
	else {
		printf_s("   ������� ����, �� �������� ����� ������� �������\n   ");
		key = getIntKey(&p);
		printf_s("   ������� ����� ������������ ������\n   ");
		number = getIntK(&p1);
		if (!(success = udalelem(ukaz, key, number, fd))) printf_s("   ������� �� ��������� ����� �� ��� ������\n\n");
		else printf_s("   ������� ��� ������� �����\n\n");
	}
	return 0;
}

int udalelem(Table* ukaz, int* key, int* number, FILE* fd) {
	copy* copy1 = (copy*)calloc(1, sizeof(copy));
	int index; int index1;
	if (*number == 2) {
		if ((index = poiskelem(ukaz, key, copy1, &index, &index1, fd)) == -1) return 0;
		else
			udalil(ukaz, &index, &index1, fd);
	}
	if (*number == 1) {
		if ((index1 = poiskelem1(ukaz, key, copy1, &index, &index1, fd)) == -1) return 0;
		else
			udalil(ukaz, &index, &index1, fd);
	}
	free(copy1);
	copy1 = NULL;
	return 1;
}

void udalil(Table* ukaz, int* index, int* index1, FILE* fd) {
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
		int i;
		ukaz->size = 0;
		for (i = 0; i < maxtablesize; i++) {
		}
	}

	void erase(Table*ukaz) {
		int i;
		for (i = 0; i < ukaz->size; i++) {
		}
	}

	FILE* openfile(Table* ukaz) {
		char* filename;
		FILE* fd;
		int choice;
		printf_s("   1. ������� ����� ����\n   2. ������� ������������ ����\n");
		printf_s("   ��� �����: ");
		getIntK(&choice);
		printf_s("\n");
		if (choice == 1) {
			printf_s("   ������� ��� �����:\n   ");
			scanf_s("%*c");
			filename = getstr();
			fopen_s(&fd, filename, "wb");
			printf_s("   ���� %s ������� ������", filename);
			printf_s("\n");
		}
		else {
			printf_s("   ������� ��� �����, ������� �� ������ �������: ");
			scanf_s("%*c");
			filename = getstr();
			printf_s("\n\n");
			if (!(fopen_s(&fd, filename, "r+b"))) {
				fread(&(ukaz->size), sizeof(int), 1, fd);
				fread(&(ukaz->keys1off), sizeof(int), 1, fd);
				fread(&(ukaz->keys2off), sizeof(int), 1, fd); int i; int chislo[3];
				for (i = 0; i < ukaz->size; i++) {
					fseek(fd, (ukaz->keys1off + 3 * i * sizeof(long)), SEEK_SET);
					fread(chislo, sizeof(long), 3, fd);
					ukaz->tab1[i].key1 = chislo[0];
					ukaz->tab1[i].offset = chislo[1];
					ukaz->tab1[i].len = chislo[2];
				}
				for (i = 0; i < ukaz->size; i++) {
					fseek(fd, (ukaz->keys2off + 3 * i * sizeof(long)), SEEK_SET);
					fread(chislo, sizeof(long), 3, fd);
					ukaz->tab2[i].key2 = chislo[0];
					ukaz->tab2[i].offset = chislo[1];
					ukaz->tab2[i].len = chislo[2];
				}
			}
			else {
				printf_s("   ���� � ����� ������ �� ������. ����� ������ ����� ����\n\n");
				fopen_s(&fd, filename, "w+b");
			}
		}
		return fd;
	}

	FILE* closefile(Table* ukaz, FILE* fd) {
		int chislo;
		rewind(fd);
		chislo = ukaz->size;
		fwrite(&chislo, sizeof(long), 1, fd);
		fseek(fd, 0, SEEK_END);
		ukaz->keys1off = ftell(fd);
		chislo = ukaz->keys1off;
		fseek(fd, sizeof(long), SEEK_SET);
		fwrite(&chislo, sizeof(long), 1, fd);
		int i;
		fseek(fd, 0, SEEK_END);
		for (i = 0; i < ukaz->size; i++) {
			chislo = ukaz->tab1[i].key1;
			fwrite(&chislo, sizeof(long), 1, fd);
			chislo = ukaz->tab1[i].offset;
			fwrite(&chislo, sizeof(long), 1, fd);
			chislo = ukaz->tab1[i].len;
			fwrite(&chislo, sizeof(long), 1, fd);
		}
		ukaz->keys2off = ftell(fd);
		for (i = 0; i < ukaz->size; i++) {
			chislo = ukaz->tab2[i].key2;
			fwrite(&chislo, sizeof(long), 1, fd);
			chislo = ukaz->tab2[i].offset;
			fwrite(&chislo, sizeof(long), 1, fd);
			chislo = ukaz->tab2[i].len;
			fwrite(&chislo, sizeof(long), 1, fd);
		}
		rewind(fd);
		fseek(fd, 2 * sizeof(long), SEEK_SET);
		chislo = ukaz->keys2off;
		fwrite(&chislo, sizeof(long), 1, fd);
		return fd;
	}
