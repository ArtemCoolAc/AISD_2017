#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <locale.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>

/*#include <fstream>
#include <regex>
#include <iostream>
#include <string>
#include <map>*/

const char *msg[] = {
	"   0 - Завершение работы",
	"   1 - Добавление вершины",
	"   2 - Добавление ребра",
	"   3 - Удаление вершины",
	"   4 - Поиск пути",
	"   5 - Вывод графа",
	"   6 - Отладочный вывод",
	"   7 - Отладочный вывод транспонированного графа",
	"   8 - Поиск сильно связанных компонент"
};

const int Nmsg = sizeof(msg) / sizeof(msg[0]);

struct Node { // данные о вершине
	int x;
	int y;
	char *name;
};

struct Element {
	int n; // количество смежных вершин
	int l;
	int color;
	int d; // метка начала
	int f; // метка конца
	Element *pred;
	Node *node;
	int *way; // указатель на динамический МАССИВ индексов смежных вершин
};

struct Graf {
	int m;
	Element* element;
	char* fname;
	int massiv[100];
	int i; // количество компонент
	int j; // количество узлов одной компоненты
};

struct Massiv {
	int index;
	int f;
};

struct Queue {
	Element *pt;
	Queue *next;
};



char *GetStr();
void GetInt(int*);
int Dialog(const char *msg[], int);
int AddNode(Graf*);
int AddEdge(Graf*);
int Delete(Graf*);
int Find(Graf*);
int Depth_First_Search(Graf*);
int GrafT_First_Search(Graf*, Graf*);
int SortMassiv(Graf*, Massiv*);
int BuildGrafT(Graf*, Graf*);
int TreesOut(Graf*, Graf*);
int ShowGrafT(Graf*);
int Strongly_Connected_Components(Graf*);
int Show(Graf*);
int ShowP(Graf*);
int SearchNode(Graf*, char*);
void InsertNode(Graf*, Node*);
int InsertEdge(Graf*, char*, char*);
void DeleteNode(Graf*, int);
void Create(Graf*);
void SaveGraf(Graf*);
void ReadFromFile(Graf*);
void RandomInsert(Graf*);
void Iniz(Graf*);
void AddInQ(Queue**, Queue**, Element*);
void CleanQueue(Queue**);
void WatchWay(Element, int);
void FreeGraf(Graf*);

int main()
{
	Graf Matr; 
	int(*fptr[])(Graf*) = { NULL, AddNode, AddEdge, Delete, Find, Show, ShowP, ShowGrafT, Strongly_Connected_Components };
	int rc;
	srand(time(NULL));
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");
	Create(&Matr);
	while (rc = Dialog(msg, Nmsg)) {
		if (!fptr[rc](&Matr))
			break;
	}
	SaveGraf(&Matr);
	return 0;
}

void GetInt(int *a) {
	int n;
	do {
		n = scanf_s("%d", a);
		if ((n == 0) && (*a < 0))
		{
			printf("   Ошибка! Повторите ввод.\n   ");
			scanf_s("%*c");
		}
	} while ((n == 0) && (*a < 0));
	printf("\n");
}

char *GetStr() {
	char *ptr = (char *)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0)
		{
			scanf("%*c");
		}
		else {
			len += strlen(buf);
			ptr = (char *)realloc(ptr, len + 2);
			strcat(ptr, buf);
		}
	} while (n > 0);
	return ptr;
}

int Dialog(const char *msg[], const int Nmsg) {
	int i, j;
	for (i = 0; i < Nmsg; ++i) // вывод действий
		puts(msg[i]);
	printf("\n   Номер выбираемого действия: ");
	GetInt(&j);
	while ((j < 0) || (j >= Nmsg)) {
		printf("   Введено неверное число, повторите ввод\n   ");
		GetInt(&j);
	}
	return j;
}

int SearchNode(Graf *table, char *name) {
	int i;
	for (i = 0; i < table->m; i++)
		if (!strcmp(table->element[i].node->name, name))
			return i;
	return -1;
}

int AddNode(Graf *table) {
	Node *node = (Node*)malloc(sizeof(Node));
	printf("   Введите имя вершины: \n   ");
	scanf("%*c");
	node->name = GetStr();
	if (SearchNode(table, node->name) != -1) {
		free(node);
		printf("   Вершина с таким именем уже существует!\n\n");
		return 1;
	}
	printf("   Введите координату Х: ");
	GetInt(&(node->x));
	printf("   Введите координату Y: ");
	GetInt(&(node->y));
	InsertNode(table, node);
	printf("   Вершина успешно добавлена!\n\n");
	return 1;
}

void InsertNode(Graf *table, Node *node) {
	(table->m)++;
	table->element = (Element*)realloc(table->element, (table->m) * sizeof(Element));
	table->element[table->m - 1].node = node;
	table->element[table->m - 1].n = 0;
	table->element[table->m - 1].way = NULL;
}

int AddEdge(Graf *table) {
	int rc;
	char *name, *name2;
	printf("   Введите имя первой вершины: \n   ");
	scanf("%*c");
	name = GetStr();
	printf("   Введите имя второй вершины: \n   ");
	name2 = GetStr();
	rc = InsertEdge(table, name, name2);
	free(name);
	free(name2);
	if (rc == -1) { // -1 - нет вершины 0 - уже есть ребро 1 - всё хорошо
		printf("   Одна из вершин не найдена! \n\n");
		return 1;
	}
	if (rc == 0) {
		printf("   Ребро уже существует! \n\n");
		return 1;
	}
	printf("   Ребро успешно добавлено! \n\n");
	return 1;
}

int InsertEdge(Graf *table, char *name, char *name2) {
	int index1 = SearchNode(table, name);
	int index2 = SearchNode(table, name2);
	int i, *list = table->element[index1].way;
	if (index1 == -1)
		return -1;  // не найдена 1 вершина
	if (index2 == -1)  // не найдена 2 вершина
		return -1;
	for (i = 0; i < table->element[index1].n; i++)
		if (list[i] == index2)  // проверка на существование ребра (дуги)
			return 0;
	if (index1 == index2) {
		(table->element[index1].n)++;
		table->element[index1].way = (int*)realloc(table->element[index1].way, (table->element[index1].n) * sizeof(int));
		table->element[index1].way[table->element[index1].n - 1] = index2;  // добавляется петля
		return 1;
	}
	(table->element[index1].n)++;
	table->element[index1].way = (int*)realloc(table->element[index1].way, (table->element[index1].n) * sizeof(int));
	table->element[index1].way[table->element[index1].n - 1] = index2; // добавляется дуга 
	//(table->element[index2].n)++;
	//table->element[index2].way = (int*)realloc(table->element[index2].way, (table->element[index2].n) * sizeof(int));
	//table->element[index2].way[table->element[index2].n - 1] = index1;  
	// если вернуть три вышестоящие строчки, то можно сделать неорграф
	return 1;
}

int Delete(Graf *table) {
	int index;
	char *name;
	printf("   Введите имя удаляемой вершины:\n");
	scanf("%*c");
	name = GetStr();
	index = SearchNode(table, name);
	if (index == -1) {
		printf("   Вершина не найдена!\n\n");
		return 1;
	}
	DeleteNode(table, index);
	printf("   Вершина успешно удалена!\n\n");
	return 1;
}

void DeleteNode(Graf *table, int index) {
	Element del = table->element[index];
	Element buf;
	int n = table->element[index].n;
	int i, j;
	for (i = 0; i < n; i++) {
		if (del.way[i] == index) // проходимся по смежным вершинам
			continue;  // если петля, то пока ничего не делаем
		buf = table->element[del.way[i]]; // смежная вершина
		j = 0;
		while (buf.way[j] != index)  // проходимся по смежным вершинам пока не найдем нашу
			j++;
		if (j != (buf.n - 1))
			buf.way[j] = buf.way[buf.n - 1];  // если не последняя вершина, то замещаем и перераспределение
		(table->element[del.way[i]].n)--;
		table->element[del.way[i]].way = (int*)realloc(table->element[del.way[i]].way, (table->element[del.way[i]].n) * sizeof(int));
	}
	table->element[index].n = table->element[table->m - 1].n;
	table->element[index].node = table->element[table->m - 1].node;
	table->element[index].way = table->element[table->m - 1].way;
	(table->m)--;
	table->element = (Element*)realloc(table->element, (table->m) * sizeof(Element));
	if (index != table->m) {
		for (i = 0; i < table->m; i++)
			for (j = 0; j < table->element[i].n; j++)
				if (table->element[i].way[j] == table->m)
					table->element[i].way[j] = index;
	}
}

int Find(Graf *table) {
	char *name, *name2, *name3;
	int i, in1, in2, in3, m = table->m;
	Element *u, finish;
	Queue *E = NULL, *Q = NULL;
	printf("   Введите имя начальной вершины:\n   ");
	scanf("%*c");
	name = GetStr();
	in1 = SearchNode(table, name);
	if (in1 == -1) {
		printf("   Вершина не найдена! \n\n");
		return 1;
	}
	printf("   Введите имя конечной вершины:\n   ");
	name2 = GetStr();
	in2 = SearchNode(table, name2);
	if (in2 == -1) {
		printf("   Вершина не найдена! \n\n");
		return 1;
	}
	printf("   Введите имя вершины, которую нужно обойти:\n   ");
	name3 = GetStr();
	in3 = SearchNode(table, name3);
	if ((in1 == in3) || (in2 == in3)) {
		printf("   Невозможно построить путь! \n\n");
		return 1;
	}
	if (in3 == -1) {
		printf("   Вершина не найдена! \n\n");
		return 1;
	}
	if (in1 == in2) {
		printf("%s --> %s\n\n", table->element[in1].node->name, table->element[in1].node->name);
		return 1;
	}
	Iniz(table);
	finish = table->element[in2];
	table->element[in1];
	table->element[in1].color = 1;
	table->element[in1].l = 0;
	AddInQ(&Q, &E, &table->element[in1]);
	while (Q != NULL) {
		u = Q->pt;
		Q = Q->next;
		for (i = 0; i < u->n; i++) {
			if (u->way[i] != in3) {
				if (table->element[u->way[i]].color == 0) {
					table->element[u->way[i]].color = 1;
					table->element[u->way[i]].l = u->l + 1;
					table->element[u->way[i]].pred = u;
					if (table->element[u->way[i]].node == finish.node) {
						printf("   Кратчайший путь: \n\n   ");
						WatchWay(table->element[u->way[i]], 0);
						printf("\n\n");
						CleanQueue(&Q);
						return 1;
					}
					AddInQ(&Q, &E, &table->element[u->way[i]]);
				}
				u->color = 2;
			}
		}
	}
	printf("   Путь не найден!\n\n");
	CleanQueue(&Q);
	return 1;
}



int DFS_Visit(Graf* table, int* ind, int* clock, int* clockprev) {
	int q = table->element[*ind].n; int j;
	table->element[*ind].color = 1;
	(*clock)++;
	for (j = 0; j < q; ++j) {
		int qq = table->element[*ind].way[j];
		if (table->element[qq].color == 0) {
			table->element[qq].pred = &(table->element[*ind]);
			DFS_Visit(table, &qq, clock, clockprev);
		}
	}
	table->element[*ind].color = 2;
	(*clock)++;
	
	
	if (((*clock) - (*clockprev)) == 1) {
		table->element[*ind].f = *clock;
		table->massiv[table->j] = *ind;
		(table->j)++;
		*clockprev = *clock;
		return 0;
	}
	table->i++;
	table->massiv[table->j] = -1;
	table->j++;
	table->massiv[table->j] = *ind;
	table->j++;
	table->element[*ind].f = *clock;
	*clockprev = *clock;
	return 0;
}

int Depth_First_Search(Graf* table) {
	Iniz(table); int i = 0; int clock = 0; int clockprev = 0;
	table->i = 0;
	table->j = 0;
	for (i; i < table->m; ++i) {
		if (table->element[i].color == 0) {
			DFS_Visit(table, &i, &clock, &clockprev);
		}
	}
	return 0;
}

int GrafT_First_Search(Graf* table, Graf* transp) {
	Iniz(transp); int i, j; int clock = 0; int clockprev = 0;
	transp->i = 0;
	transp->j = 0;
	Massiv* mas = (Massiv*)calloc(table->m, sizeof(Massiv));
	for (i = 0; i < table->m; ++i) {
		mas[i].index = i;
		mas[i].f = table->element[i].f;
	}
	SortMassiv(table, mas);
	for (i = 0; i < table->m; ++i) {
		j = mas[i].index;
		if (transp->element[j].color == 0) {
			DFS_Visit(transp, &j, &clock, &clockprev);
		}
	}
	transp->massiv[transp->j] = -1;
	return 0;
}

int SortMassiv(Graf* table, Massiv* mas) {
	int i; int j; Massiv tmp;
	for (i = 1; i < table->m; ++i) {
		for (j = 1; j < table->m; ++j) {
			if (mas[j].f > mas[j - 1].f) {
				tmp = mas[j];
				mas[j] = mas[j - 1];
				mas[j - 1] = tmp;
			}
		}
	}
	return 0;
}

int BuildGrafT(Graf* table, Graf* transp) {
	int i, j, k;
	transp->m = table->m;
	transp->fname = table->fname;
	for (i = 0; i < table->m; ++i) {
		transp->element = (Element*)calloc(table->m, sizeof(Element));
		transp->element[i].node = (Node*)calloc(1, sizeof(Node));
		transp->element[i].node->name = table->element[i].node->name;
		transp->element[i].node->x = table->element[i].node->x;
		transp->element[i].node->y = table->element[i].node->y;
	}
	for (j = 0; j < table->m; ++j) { // для каждой вершины производится поиск по всем вершинам
		for (k = 0; k < table->element[j].n; k++) {
			i = table->element[j].way[k];
			(transp->element[i].n)++;
			transp->element[i].way = (int*)realloc(transp->element[i].way, (transp->element[i].n) * sizeof(int));
			transp->element[i].way[transp->element[i].n - 1] = j;
		}
		//		ShowP(transp);
	}
	return 0;
}

int TreesOut(Graf* transp, Graf* table) {
	int i;  int j = 1; printf("  ");
	for (i = 0; i < transp->i; ++i) {
		while (transp->massiv[j] != -1) {
			printf(" %d (%s)", transp->massiv[j], table->element[transp->massiv[j]].node->name);
			j++;
		}
		j++;
		printf("\n  ");
	}
	printf("\n");
	return 0;
}

int ShowGrafT(Graf* table) {
	Graf transp;
	BuildGrafT(table, &transp);
	ShowP(&transp);
	return 1;
}

int Strongly_Connected_Components(Graf* table) {
	Graf transp; int i;
	Depth_First_Search(table);
	BuildGrafT(table, &transp);
	GrafT_First_Search(table, &transp);
	TreesOut(&transp, table);
	free((&transp)->element);
	return 1;
}

int Show(Graf *table) {
	int i, j;
	for (i = 0; i < table->m; i++) {
		printf("   x = %d, y = %d, name = %s.\n", table->element[i].node->x, table->element[i].node->y, table->element[i].node->name);
		printf("   Смежные вершины:\n");
		for (j = 0; j < table->element[i].n; j++) {
			printf("\t\t%s\n", table->element[table->element[i].way[j]].node->name);
		}
		printf("\n");
	}
	return 1;

}

int ShowP(Graf* table) {
	int i, j;
	for (i = 0; i < table->m; i++) {
		printf("   Вершина %d: ", i);
		for (j = 0; j < (table->element[i].n); ++j)
			printf(" %d ", table->element[i].way[j]);
		printf("\n");
	}
	printf("\n");
	return 1;
}

void Create(Graf *table) {
	FILE *fp;
	int f;
	table->m = 0;
	table->element = NULL;
	printf("   Введите имя файла:\n   ");
	table->fname = GetStr();
	fp = fopen(table->fname, "r+b");
	if (fp != NULL) {
		ReadFromFile(table);
		return;
	}
	printf("   Файл не найден, создан новый файл! \n\n");
	fp = fopen(table->fname, "w+b");
	printf("   Построить граф случайным образом?(1/0)");
	do {
		GetInt(&f);
		if (f == 1) {
			RandomInsert(table);
			return;
		}
		if (f == 0)
			return;
	} while ((f != 1) && (f != 0));
}

void ReadFromFile(Graf *table) {
	int i, j, len;
	Node *node;
	FILE *fp = fopen(table->fname, "r+b");
	fread(&(table->m), sizeof(int), 1, fp);
	table->element = (Element*)malloc((table->m) * sizeof(Element));
	for (i = 0; i < table->m; i++) {
		node = (Node*)malloc(sizeof(Node));
		fread(&(node->x), sizeof(int), 1, fp);
		fread(&(node->y), sizeof(int), 1, fp);
		fread(&len, sizeof(int), 1, fp);
		node->name = (char*)malloc(len * sizeof(char));
		fread(node->name, sizeof(char), len, fp);
		fread((&table->element[i].n), sizeof(int), 1, fp);
		table->element[i].node = node;
		table->element[i].way = (int*)malloc((table->element[i].n) * sizeof(int));
		for (j = 0; j < table->element[i].n; j++)
			fread(&(table->element[i].way[j]), sizeof(int), 1, fp);
	}
	fclose(fp);
	printf("   Граф успешно считан! \n\n");

}

/*void ReadFromFile2(Graf* table) {
	int i, j, len;
	Node *node;
	std::ifstream file("Graf.txt"); std::string buf;
	std::regex reg("\[\[(\\d), '\\w'\], (\\d)]", std::regex::icase);
	std::smatch m;
	std::multimap<const char*, const char*> transitions; int s = 0;
	std::string b1, b2;
	while (std::getline(file, buf)) {
		bool res = std::regex_search(buf, m, reg);
		if (res) {
			b1 = m[1];
			b2 = m[2];
			transitions.insert(b1, b2);
			++s;
		}
	}
	table->m = s;
	table->element = (Element*)malloc((table->m) * sizeof(Element));
	auto iter = transitions.begin();
	for (i = 0; i < table->m; i++) {
		node = (Node*)malloc(sizeof(Node));
		node->x = 1000;
		node->y = 1000;

	}
}*/

void SaveGraf(Graf *table) {
	int i, j, len;
	FILE *fp = fopen(table->fname, "w+b");
	fwrite(&(table->m), sizeof(int), 1, fp);
	for (i = 0; i < table->m; i++) {
		fwrite(&(table->element[i].node->x), sizeof(int), 1, fp);
		fwrite(&(table->element[i].node->y), sizeof(int), 1, fp);
		len = strlen(table->element[i].node->name) + 1;
		fwrite(&len, sizeof(int), 1, fp);
		fwrite(table->element[i].node->name, sizeof(char), len, fp);
		fwrite(&(table->element[i].n), sizeof(int), 1, fp);
		for (j = 0; j < table->element[i].n; j++) {
			fwrite(&(table->element[i].way[j]), sizeof(int), 1, fp);
		}
		free(table->element[i].node->name);
		free(table->element[i].node);
		free(table->element[i].way);
	}
	free(table->element);
	fclose(fp);
	printf("   Граф сохранен в файл!\n   ");
}

void RandomInsert(Graf *table) {
	Node *node;
	char *name1, *name2;
	int m, i, j, n, k = 0;
	printf("   Введите количество вершин: ");
	GetInt(&m);
	for (i = 0; i < m; i++) {
		node = (Node*)malloc(sizeof(Node));
		node->x = rand() % 200 - 100;
		node->y = rand() % 200 - 100;
		node->name = (char*)malloc(5 * sizeof(char));
		n = rand() % 100 + 1000;
		_itoa(n, node->name, 10);
		if (SearchNode(table, node->name) != -1) {
			i--;
			free(node);
		}
		else
			InsertNode(table, node);
	}
	n = ((m - 1) * ((rand() % (m - 1)) + 1)) / 2;
	while (k < n) {
		i = rand() % m;
		j = rand() % m;
		if (i != j) {
			name1 = table->element[i].node->name;
			name2 = table->element[j].node->name;
			if (InsertEdge(table, name1, name2) == 1)
				k++;
		}
	}
	printf("   Граф успешно создан!\n\n");
}

void Iniz(Graf *table) {
	int i;
	for (i = 0; i < table->m; i++) {
		table->element[i].color = 0;
		table->element[i].l = INT_MAX;
		table->element[i].pred = NULL;
	}
}

void AddInQ(Queue **Q, Queue **E, Element *el) {
	Queue *buf = (Queue*)malloc(sizeof(Queue));
	buf->pt = el;
	buf->next = NULL;
	if (*Q == NULL) {
		*Q = buf;
		*E = buf;
	}
	else {
		(*E)->next = buf;
		*E = (*E)->next;
	}
}

void CleanQueue(Queue **Q) {
	Queue *buf;
	while (*Q != NULL) {
		buf = *Q;
		*Q = (*Q)->next;
		free(buf);
	}
}

void WatchWay(Element el, int f) {
	if (el.pred != NULL)
		WatchWay(*el.pred, 1);
	if (f)
		printf("%s --> ", el.node->name);
	else
		printf("%s", el.node->name);
}

