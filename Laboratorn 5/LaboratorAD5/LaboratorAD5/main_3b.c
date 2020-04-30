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


// функции ввода:
char *getStr();	// строки произвольной длины
int getInt(int *);	// целого
void ignore();
int insert (Table *, int k, char *);	// вставка элемента
int delItem(Table *, int);		// удаление элемента
int find(Table *, int, int rel);// поиск элемента
int findI(Table *, int *);// поиск rel элемента для добавления
void putTable(Table);		// вывод содержимого таблицы
void delTable(Table *);		// освобождение памяти

// Альтернативы меню для организации диалога
const char *msgs[] = {"0. Выход", "1. Добавить элемент", "2. Найти элемент", "3. Удалить элемент", "4. Отобразить таблицу"};	// список альтернатив
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);	// количество альтернатив

// функции для организации диалога;
// при обнаружении конца файла возвращают 0
int dialog(const char *msgs[], int);	// выбор номера альтернативы
int D_Add(Table *),		// вставка элемента в таблицу
    D_Find(Table *),		// поиск элемента в таблице
    D_Delete(Table *),	// удаление элемента из таблицы
    D_Show(Table *);		// вывод содержимого таблицы

// массив указателей на функции - для реализации выбора функции
// порядок перечисления функций должен соответствовать
// порядку указания альтернатив в списке альтернатив
int (*fptr[])(Table *) = {NULL, D_Add, D_Find, D_Delete, D_Show};


///Функция инициализации таблицы нулями
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
            break; 	// при выполнении диалоговой функции обнаружен конец файла
        system("pause");}
    delTable(&table);
    printf("Работа завершена\n");
    return 0;
}


/// Функция для выбора номера альтернативы.
/// Возвращает правильный номер альтернативы.
int dialog(const char *msgs[], int N)
{
    char *errmsg = "";
    int rc;
    int i, n;

    do{
        puts(errmsg);
        errmsg = "Ошибка. Повторите попытку";

        // вывод списка альтернатив
        for(i = 0; i < N; ++i)
            puts(msgs[i]);
        puts("Введите номер нужного вам действия\n");

        n = getInt(&rc); // ввод номера альтернативы
        if(n == 0) // конец файла - конец работы
            rc = 0;
    } while(rc < 0 || rc >= N);

    return rc;
}

const char *errmsgs[] = {"Ok", "Таблица переполнена"};


/// Диалоговая функция включения в таблицу нового элемента.
/// Требует ввести ключ и информацию, при этом информация должна быть
/// введена в новой строке, в ответ на приглашение.
/// Если ключ задан неправильно, вся строка игнорируется
/// Если при вводе ключа или информации обнаруживается конец файла,
/// функция возвращает значение 0.
/// При успешном завершении функция возвращает значение 1.
int D_Add(Table *ptab)
{
    int k, rc, n;
    char *info = NULL;
    puts("Введите ключ: -->");
    n = getInt(&k);
    if(n == 0)
        return 0; // обнаружен конец файла
    ignore(); // удаляет все оставшиеся символы из буфера ввода

    puts("Введите информацию:");
    info = getStr(); // вся строка вводится целиком
    if(info == NULL)
        return 0;	// обнаружен конец файла

    rc = insert(ptab, k, info);// вставка элемента в таблицу
    free(info); // если элемент вставляется в таблицу - вставляется его копия

    printf("%s\n", errmsgs[rc]);
    return 1;
}

/// Диалоговая функция поиска элемента в таблице по ключу
int D_Find(Table *ptab)
{
    int key, index, rel, flag,rel1, k;
    Item* item;
    puts("Введите ключ--->");
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
            printf("Ключ: %d, rel: %d, Данные: \"%s\"\n", item->key, item->release, item->info);
            flag=0;
        }
        else
            if (flag)
            {
                printf("Ошибка! Ключ не найден %d\n", k);
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

/// диалоговая функция удаления из таблицы элемента, заданного ключом
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

/// диалоговая функция вывода содержимого таблицы
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
        puts("Таблица пуста");;
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
            printf("Ошибка! Повторите ввод!\n");
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

///Функция вставки элементов в таблицу
///Получает на вход указатель на таблицу, ключ и строку
///Возвращает код выполнения операции
int insert (Table *ptab, int k, char *info)
{
    int key, rel;
//    char *delchar = NULL;
    Item *p;
    if (ptab->size >= maxsize)
        return 1;//проверка на переполнение таблицы
    key = k;
    rel = findI(ptab, &key);
    if (!ptab->kspc[key].item)
    {
        p = (Item *)calloc(sizeof(Item), 1); //создание нового элемента
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
    int rel = 0, key, i, h2 = 1 + (*k < 0?-*k:*k) % (maxsize / 2), //на случай, если вдруг maxsize == 1
    h1 = (*k < 0?-*k:*k) % maxsize, ret=-1;
    key = h1;
    i = 0;
    while (i < maxsize && ptab->kspc[key].item) //добавление в пространство ключей
    {
        if (ptab->kspc[key].item && ptab->kspc[key].item->busy && ptab->kspc[key].item->key == *k && ptab->kspc[key].item->release > rel)
            rel = ptab->kspc[key].item->release; //найден в i пр-ве
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

///получает на вход ключ и номер релиза
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

/// Функция удаления остатка строки
void ignore()
{
    scanf("%*[^\n]");
    scanf("%*c");
}


///очистка таблицы
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
