#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>

const int maxsize = 10;
struct Item {
	int busy;
	int key;
	int par;
	int offset;
	int len;
};
struct Table {
	int size;
	int vector;
	Item tab[maxsize];
};

// ������� �����:
char *getStr();	// ������ ������������ �����
int getInt(int *);
void ignore();
int insert(FILE *, Table *, int k, char *, int);	// ������� ��������
int findADD(Table *, int);// ����� �������� ��� ������� �������
Item *findINSERT(Table *, int);//����� ���������� �����
int find(FILE *,Table *, int);//����� �����
void show(FILE *, Table *);		// ����� ����������� �������
void deleteKEY( Table *,int);//�������� ���� � �����
void delTable(Table *);		// ������������ ������
int hash(int);
FILE *openfile(Table *);
void closefile(FILE *, Table *);

const char *msgs[] = { "0. �����", "1. �������� �������", "2. ����� �����", "3. �������", "4. ������� �������" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int dialog(const char *msgs[], int);
int D_Add(FILE *, Table *),
D_Find(FILE *, Table *),
D_Delete(FILE *, Table *),
D_Show(FILE *, Table *);
int(*fptr[])(FILE *, Table *) = { NULL, D_Add, D_Find, D_Delete, D_Show };

void initTab(Table *tbl)
{
	int i;
	for (i = 0; i <maxsize; i++)
	{
		tbl->tab[i].offset = 0;
		tbl->tab[i].busy = 0;
		tbl->tab[i].key = 0;
		tbl->tab[i].par = 0;
		tbl->tab[i].len = 0;
	}
	tbl->size = 0;
	tbl->vector = 0;
}
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	int rc;
	Table tbl;
	FILE *fd = openfile(&tbl);
	while (rc = dialog(msgs, NMsgs))
	{
		if (!fptr[rc](fd,&tbl))
			break; 	// ��� ���������� ���������� ������� ��������� ����� �����
	}
	closefile(fd, &tbl);
	printf("����\n");
	return 0;
}

int dialog(const char *msgs[], int N)
{
	char *errmsg = "";
	int rc, i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeat, please!";
		for (i = 0; i < N; ++i)
			puts(msgs[i]);
		puts("������� ����� ��������: --> ");
		n = getInt(&rc); // ���� ������ ������������
		if (n == 0) // ����� ����� - ����� ������
			rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}
const char *errmsgs[] = { "Ok", "������� �����������","����� ������� ����","��� ������ ��������" };
int D_Add(FILE *fd, Table *tbl)
{
	int key, rc, n, par;
	char *info = NULL;
	puts("������� ����: -->");
	n = getInt(&key);
	if (n == 0)
		return 0; // ��������� ����� �����
	ignore(); // ������� ��� ���������� ������� �� ������ �����
	n = findADD(tbl, key);
	switch (n)
	{
	case 1: {
		rc = 2;
		break;
	}
	case -1: {
		puts("������� ����������:");
		info = getStr(); // ��� ������ �������� �������
		if (info == NULL)
			return 0;	// ��������� ����� �����
		puts("������� ���� ��������");
		getInt(&par);
		n = findADD(tbl, par);
		if (n == 1)
			rc = insert(fd,tbl, key, info, par);// ������� �������� � �������
		else
			rc = 3;
		info = NULL;
	}
	}
	printf("%s\n", errmsgs[rc]);
	return 1;
}
int D_Find(FILE *fd, Table *tbl)
{
	int k, n;
	puts("������� ���� ��������");
	getInt(&k);
	n = find(fd,tbl, k);
	if (n == 0)
		puts("��� �����");
	return 1;
}
int D_Delete(FILE *fd, Table *tbl)
{
	int key;
	puts("������� ��������� ����");
	getInt(&key);
	deleteKEY(tbl,key);
	puts("OK");
	return 1;
}
int D_Show(FILE *fd, Table *tbl)
{
	puts("�������--->\n");
	show(fd,tbl);
	return 1;
}

int getInt(int *p)
{
	int n;
	do
	{
		n = scanf_s("%d", p);
		if (n < 0)
			return 0;
		if (n == 0)
		{
			printf("error! Repeat input!\n");
			scanf_s("%*[^\n]");
		}
	} while (n == 0);
	return 1;
}
char *getStr()
{
	char buf[21];
	int n, len = 0, curLen;
	char *ptr = (char *)malloc(1);
	*ptr = '\0';
	do
	{
		n = scanf_s("%20[^\n]", buf,21);
		if (n<0)
		{
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n>0)
		{
			curLen = strlen(buf);
			len += curLen;
			ptr = (char *)realloc(ptr, len + 1);
			strcat_s(ptr, len + 1, buf);
		}
		else
			scanf_s("%*c");
	} while (n>0);
	return ptr;
}
void ignore()
{
	scanf_s("%*[^\n]");
	scanf_s("%*c");
}

void deleteKEY( Table *tbl,int k) {
	int i = hash(k), j = hash(k);
	tbl->size = tbl->size - 1;
	do {
		if ((tbl->tab[i].key == k) && (tbl->tab[i].busy == 1))
		{
			tbl->tab[i].busy = -1;
			continue;
		}
		i = (i + 1) % maxsize;
	} while ((i != j) && (tbl->tab[i].busy != 0));
	for (i = 0; i < maxsize; ++i) {
		if ((tbl->tab[i].par == k) && (tbl->tab[i].busy == 1))
			deleteKEY(tbl, tbl->tab[i].key);
	}
}
int insert(FILE *fd, Table *tbl, int key, char *info, int par)
{
	Item *p;
	if (tbl->size >= maxsize)
		return 1;//�������� �� ������������ �������
	p = findINSERT(tbl, key);
	p->busy = 1;
	p->key = key;
	p->par = par;
	p->len = strlen(info) + 1;
	fseek(fd, 0L, SEEK_END);
	p->offset = ftell(fd);
	fwrite(info, sizeof(char), p->len, fd);
	tbl->size = tbl->size + 1;
	tbl->vector = tbl->vector + p->len;
	return 0;
}
int findADD(Table *tbl, int key)
{
	int i = hash(key),j=hash(key);
	if (tbl->size != 0)
		do{
			if ((key == (tbl->tab[i].key)) && (tbl->tab[i].busy != -1))
			{
				return 1;
				continue;
			}
			i = (i + 1) % maxsize;
		} while ((i != j) && (tbl->tab[i].busy != 0));
	if (key == 0) return 1;
	return -1;
}
Item *findINSERT(Table *tbl, int key)
{
	int i = hash(key), j = hash(key);
	if (tbl->size != 0)
		do {
			if (tbl->tab[i].busy != 1)
				return &(tbl->tab[i]);
			i = (i + 1) % maxsize;
		} while (i != j);
	else
		return &(tbl->tab[i]);
	return 0;
}
int find(FILE *fd, Table *tbl, int k) {
	int i, n = 0;
	//Item *prt;
	char *str;
	//str = (char*)malloc(tbl->tab[i].len);
	for (i = 0; i < maxsize; ++i) {
		if (tbl->tab[i].par == k) {
			str = (char*)malloc(tbl->tab[i].len);
			fseek(fd, tbl->tab[i].offset, SEEK_SET);
			fread(str, sizeof(char), tbl->tab[i].len, fd);
			printf_s("key %d par %d info %s \n", tbl->tab[i].key, tbl->tab[i].par,str);
			free(str);
			n = 1;
		}
	}
	return n;
}
int hash(int k) {
	return k%maxsize;
}
void show(FILE *fd,Table *tbl) {
	int i;
	char *str = (char *)malloc(1);;
	if (tbl->size != 0)
		for (i = 0; i < maxsize; ++i) {
			if (tbl->tab[i].busy == 1) {
				str = (char*)realloc(str,tbl->tab[i].len+1);
				fseek(fd, tbl->tab[i].offset, SEEK_SET);
				fread(str, sizeof(char), tbl->tab[i].len, fd);
				printf_s("%d ���� %d �������� %d ���������� \"%s\"\n", i, tbl->tab[i].key, tbl->tab[i].par,str);
			}
			else
				printf_s("%d ������� ����\n", i);
		}
	else
		puts("������� �����");
	free(str);
}

FILE *openfile(Table *tbl) {
	char *filename;
	FILE *fd;
	int v;
	puts("������� ������������ ���� ��� ������� �����?(1/0)");
	getInt(&v);
	while ((v != 1) && (v != 0)) {
		puts("���������� �����");
		getInt(&v);
	}
	ignore();
	if (v == 0) {
		puts("�������� �������� ������ �����");
		filename = getStr();
		fopen_s(&fd,filename,"w+b");
		printf_s("������ ���� %s",filename);
		initTab(tbl);
		fwrite(&(tbl->vector), sizeof(int), 1, fd);
		fwrite(&(tbl->size), sizeof(int), 1, fd);
		fwrite(tbl, sizeof(Item), maxsize, fd);
	}
	else {
			puts("������� �������� �����, ������� ������ �������");
			filename = getStr();
			if (!(fopen_s(&fd, filename, "r+b"))) {
				fread(&(tbl->vector), sizeof(int), 1, fd);
				fread(&(tbl->size), sizeof(int), 1, fd);
				fread(tbl, sizeof(Item), maxsize, fd);

				printf_s("%d %d", tbl->vector, tbl->size);
			}
			else
			{
				puts("���-�� ����� �� ���");
				fopen_s(&fd, filename, "w+b");
				printf_s("������ ���� %s", filename);
				initTab(tbl);
				fwrite(&(tbl->vector), sizeof(int), 1, fd);
				fwrite(&(tbl->size), sizeof(int), 1, fd);
				fwrite(tbl, sizeof(Item), maxsize, fd);
			}

	}
	free(filename);
	return fd;
}
void closefile(FILE *fd, Table *tbl) {
	if (tbl->size) {
		fseek(fd, 0, SEEK_SET);
		fwrite(&(tbl->vector), sizeof(int), 1, fd);
		fwrite(&(tbl->size), sizeof(int), 1, fd);
		fwrite(tbl, sizeof(Item), maxsize, fd);
	}
	fclose(fd);
}
