#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

const char *msg[] = { "   0. �����", "   1. �������� �������", "   2. ������� �������",  "   3. ��������� ����������","   4. ������ ����� �� �����" }; // �������������� ��������
const int Nmsg = sizeof(msg) / sizeof(msg[0]);    // ���������� ��������

struct Node {  //��������� ����
	int x;
	int y;
	char* name;
	int colour;
};

struct List {  //������ ���������
	int n;
	int number;
	Node* next;
};


struct Graph {  // �������
	int m;
	List* start;
	char* name;
};

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Rus");

}