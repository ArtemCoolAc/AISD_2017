#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

const char *msg[] = { "   0. Выход", "   1. Добавить элемент", "   2. Удалить элемент",  "   3. Рандомное заполнение","   4. Чтение графа из файла" }; // альтернативные действия
const int Nmsg = sizeof(msg) / sizeof(msg[0]);    // количество действий

struct Node {  //структура узла
	int x;
	int y;
	char* name;
	int colour;
};

struct List {  //список смежности
	int n;
	int number;
	Node* next;
};


struct Graph {  // матрица
	int m;
	List* start;
	char* name;
};

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");

}