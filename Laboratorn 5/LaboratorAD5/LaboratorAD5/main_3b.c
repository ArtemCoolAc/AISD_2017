#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>


const int maxsize = 11;


typedef struct Item{
    int key;
    int busy;
    int release;
    char *info;
} Item;

typedef struct krcd{
    Item *item;
} krcd;

typedef struct Table{
    int size;
    krcd *kspc;
} Table;


// ������� �����:
char *getStr();	// ������ ������������ �����
int getInt(int *);	// ������
void ignore();
int insert (Table *, int k, char *);	// ������� ��������
int delItem(Table *, int);		// �������� ��������
int find(Table *, int, int rel);// ����� ��������
int findI(Table *, int *);// ����� rel �������� ��� ����������
void putTable(Table);		// ����� ����������� �������
void delTable(Table *);		// ������������ ������

// ������������ ���� ��� ����������� �������
const char *msgs[] = {"0. �����", "1. �������� �������", "2. ����� �������", "3. ������� �������", "4. ���������� �������"};	// ������ �����������
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);	// ���������� �����������

// ������� ��� ����������� �������;
// ��� ����������� ����� ����� ���������� 0
int dialog(const char *msgs[], int);	// ����� ������ ������������
int D_Add(Table *),		// ������� �������� � �������
    D_Find(Table *),		// ����� �������� � �������
    D_Delete(Table *),	// �������� �������� �� �������
    D_Show(Table *);		// ����� ����������� �������

// ������ ���������� �� ������� - ��� ���������� ������ �������
// ������� ������������ ������� ������ ���������������
// ������� �������� ����������� � ������ �����������
int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show};


///������� ������������� ������� ������
void initTab(Table *ptab)
{
    int i;
    krcd *p = ptab->kspc;
    for (i = 0; i <maxsize; i++, p++)
    {
        p->item = NULL;
    }
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Table table = {0, (krcd *)calloc(maxsize, sizeof(krcd))};
    int rc;
    initTab(&table);
    while(rc = dialog(msgs, NMsgs))
        {if(!fptr[rc](&table))
            break; 	// ��� ���������� ���������� ������� ��������� ����� �����
        system("pause");}
    delTable(&table);
    printf("������ ���������\n");
    return 0;
}


/// ������� ��� ������ ������ ������������.
/// ���������� ���������� ����� ������������.
int dialog(const char *msgs[], int N)
{
    char *errmsg = "";
    int rc;
    int i, n;

    do{
        puts(errmsg);
        errmsg = "������. ��������� �������";

        // ����� ������ �����������
        for(i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("������� ����� ������� ��� ��������\n");

        n = getInt(&rc); // ���� ������ ������������
        if(n == 0) // ����� ����� - ����� ������
            rc = 0;
    } while(rc < 0 || rc >= N);

    return rc;
}

const char *errmsgs[] = {"Ok", "������� �����������"};


/// ���������� ������� ��������� � ������� ������ ��������.
/// ������� ������ ���� � ����������, ��� ���� ���������� ������ ����
/// ������� � ����� ������, � ����� �� �����������.
/// ���� ���� ����� �����������, ��� ������ ������������
/// ���� ��� ����� ����� ��� ���������� �������������� ����� �����,
/// ������� ���������� �������� 0.
/// ��� �������� ���������� ������� ���������� �������� 1.
int D_Add(Table *ptab)
{
    int k, rc, n;
    char *info = NULL;
    puts("������� ����: -->");
    n = getInt(&k);
    if(n == 0)
        return 0; // ��������� ����� �����
    ignore(); // ������� ��� ���������� ������� �� ������ �����

    puts("������� ����������:");
    info = getStr(); // ��� ������ �������� �������
    if(info == NULL)
        return 0;	// ��������� ����� �����

    rc = insert(ptab, k, info);// ������� �������� � �������
    free(info); // ���� ������� ����������� � ������� - ����������� ��� �����

    printf("%s\n", errmsgs[rc]);
    return 1;
}

/// ���������� ������� ������ �������� � ������� �� �����
int D_Find(Table *ptab)
{
    int key, index, rel, flag,rel1, k;
    Item* item;
    puts("������� ����--->");
    if(!getInt(&key))
        return 0;
    ignore();

    printf("Enter release (if you want see all releases , then enter %d) --->\n",maxsize+1);
    if(!getInt(&rel))
        return 0;
    ignore();
    k=key;
    rel1= findI(ptab,&key);
    if (rel == maxsize+1)
    {
        rel=1;
        flag=1;
        do {
            index = find(ptab, k, rel);
            item = ptab->kspc[index].item;
        if ((index+1) && item)
        {
            printf("����: %d, rel: %d, ������: \"%s\"\n", item->key, item->release, item->info);
            flag=0;
        }
        else
            if (flag)
            {
                printf("������! ���� �� ������ %d\n", k);
            }
        rel++;
        } while ((index+1) || (rel<=rel1));
    }
    else
    {
    index = find(ptab, k, rel);
    item = ptab->kspc[index].item;
    if ((index+1) && item)
        printf("key: %d, rel: %d, info: \"%s\"\n", item->key, item->release, item->info);
    else
        printf("Error! Can not find key %d\n", k);
    }
    return 1;
}

/// ���������� ������� �������� �� ������� ��������, ��������� ������
int D_Delete(Table *ptab)
{
    int key, index, rel;
    puts("Enter key--->");
    if(!getInt(&key))
        return 0;
    ignore();
    puts("Enter release--->");
    if(!getInt(&rel))
        return 0;
    ignore();
    index = find(ptab, key, rel);
        if ((index + 1) && (ptab->kspc[index].item))
        {
            ptab->kspc[index].item->busy = 0;
            //ptab->kspc[index].item->info = NULL;
            //ptab->kspc[index].item = NULL;
            //free(ptab->kspc[index].item);
            printf("Ok\n");
            ptab->size--;
        }
        else
            printf("Error! Can not find key %d\n", key);

    return 1;
}

/// ���������� ������� ������ ����������� �������
int D_Show(Table *ptab)
{
    krcd *a = (ptab->kspc);
    int i, k = 0;
    for(i = 0; i < maxsize; ++i, ++a)
    {
        if (a->item && a->item->info && a->item->busy)
            {printf("#%d, key: %d, rel: %d, info: \"%s\"\n",i, a->item->key, a->item->release, a->item->info);
            k++;}
            //else
                //printf("#%d - del: %d\n", i, a->del);
    }
     if (!k)
        puts("������� �����");;
    return 1;
}

int getInt(int *p)
{
    int n;
    do
    {
        n = scanf("%d", p);
        if (n < 0)
            return 0;
        if (n == 0)
        {
            printf("������! ��������� ����!\n");
            scanf("%*[^\n]");
        }
    }
    while (n == 0);
    return 1;
}

char *getStr()
{
    char buf[21];
    int n, len = 0, cLen;
    char *pt = (char *)malloc(1);
    *pt = '\0';
    do
    {
        n = scanf("%20[^\n]", buf);
        if (n<0)
        {
            free(pt);
            pt = NULL;
            continue;
        }
        if (n>0)
        {
            cLen = strlen(buf);
            len += cLen;
            pt = (char *)realloc(pt, len+1);
            strcat(pt, buf);
        }
        else
            scanf("%*c");
    }
    while (n>0);
    return pt;
}

///������� ������� ��������� � �������
///�������� �� ���� ��������� �� �������, ���� � ������
///���������� ��� ���������� ��������
int insert (Table *ptab, int k, char *info)
{
    int key, rel;
//    char *delchar = NULL;
    Item *p;
    if (ptab->size >= maxsize)
        return 1;//�������� �� ������������ �������
    key = k;
    rel = findI(ptab, &key);
    if (!ptab->kspc[key].item)
    {
        p = (Item *)calloc(sizeof(Item), 1); //�������� ������ ��������
    }
    else
        p = ptab->kspc[key].item;
    p->key = k;
    p->info = (char *)realloc(p->info, strlen(info)+1);
    p->busy = 1;
    p->release = rel + 1;
    *p->info = '\0';
    strcat(p->info, info);
    ptab->kspc[key].item = p;
    ptab->size++;
    return 0;
}

int findI(Table *ptab, int *k)
{
    int rel = 0, key, i, h2 = 1 + (*k < 0?-*k:*k) % (maxsize / 2), //�� ������, ���� ����� maxsize == 1
    h1 = (*k < 0?-*k:*k) % maxsize, ret=-1;
    key = h1;
    i = 0;
    while (i < maxsize && ptab->kspc[key].item) //���������� � ������������ ������
    {
        if (ptab->kspc[key].item && ptab->kspc[key].item->busy && ptab->kspc[key].item->key == *k && ptab->kspc[key].item->release > rel)
            rel = ptab->kspc[key].item->release; //������ � i ��-��
        i++;
        if (ptab->kspc[key].item && !ptab->kspc[key].item->busy && key>ret)
            ret=key;
        key = (h1 + i*h2) % maxsize;
    }
    if (ret==-1)
        *k = key;
    else
        *k=ret;
    return rel;
}

///�������� �� ���� ���� � ����� ������
int find(Table *ptab, int k, int rel)
{
    int i = 0, key;
    int h2 = 1 + (k < 0?-k:k) % (maxsize / 2);
    int h1 = (k < 0?-k:k) % maxsize;
    key = h1;
    while (i < maxsize)
    {
            if (ptab->kspc[key].item && ptab->kspc[key].item->busy && ptab->kspc[key].item->key == k && ptab->kspc[key].item->release == rel)
                return key;
            if (!ptab->kspc[key].item)
                return -1;
            key = (key + 1) % maxsize;
            i++;
            key = (h1 + i*h2) % maxsize;
    }
    return -1;
}

/// ������� �������� ������� ������
void ignore()
{
    scanf("%*[^\n]");
    scanf("%*c");
}


///������� �������
void delTable(Table *ptab)
{
    int i;
    krcd *p = ptab->kspc;
    for (i = 0; i < maxsize; i++, p++)
    {
                if (p->item)
                    free(p->item->info);
                free(p->item);
    }
    free(ptab->kspc);
}
