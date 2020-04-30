#include <stdio.h> 
#include <malloc.h> 
#include <stdlib.h>
#include <locale.h>
#include <conio.h>

void getInt(int*);
void getIntPol(int*);
struct Stroki {
	int n;
	int *mat;
};
struct Stolb {
	int m;
	struct Stroki *str;
};
void getStolb(struct Stolb*);
void getStroki(struct Stroki*);
void setMassiv(struct Stolb*);
void erase(struct Stolb*);
void change(struct Stolb*, int, struct Stroki*);
void obrabotka(struct Stolb*);
void CopyMatr(struct Stolb*, struct Stolb*);
int ObrabotkaChisla(int);
int main()
{
	struct Stolb stlb; 
	struct Stolb stlb2; 
	struct Stroki newstroka;
	setlocale(LC_ALL, "Rus");
	getStolb(&stlb);
	CopyMatr(&stlb, &stlb2);
	printf("Это начальная матрица:\n"); 
	setMassiv(&stlb);
	obrabotka(&stlb2);
	printf("Это матрица после преобразования:\n"); 
	setMassiv(&stlb2);
	erase(&stlb); 
	erase(&stlb2);
	_getch();
	return 0;
}

void getStolb(struct Stolb *pa)
{
	int mm;
	printf("Введите количество строк: "); 
	getIntPol(&mm);
	pa->m = mm;
	pa->str = ((struct Stroki*)calloc(mm, sizeof(struct Stroki)));
	struct Stroki* ptr = pa->str;
	while (mm-- > 0)
	{
		getStroki(ptr++);
	}
}

void getStroki(struct Stroki *pm)
{
	int  nn, znach; 
	int *ptr; 
	printf("Введите количество элементов в строке: ");
	getIntPol(&nn);
	pm->n = nn;
	pm->mat = ((int*)calloc(nn, sizeof(int*)));
	ptr = pm->mat; 
	printf("Введите %d чисел:\n", nn);
		while (nn-- > 0) 
		{
		getInt(&znach);
		*(ptr++) = znach;
	}
}

void setMassiv(struct Stolb *uk)
{
	int j = uk->m; 
	struct Stroki* ukr = uk->str; 
	while (j-- > 0)
	{
	int nn = ukr->n; 
	int *p1 = ukr->mat; //указатель на элементы в строке
	ukr++;
		while (nn-- >0)
		{
			printf("%d ", *p1++);
		}
		printf("\n");
		
	}
	
}

void change(struct Stolb *st, int i, struct Stroki* newline)
{
	struct Stroki* stol = (st->str) + i;
	stol = newline;
	newline = NULL;

}

void erase(struct Stolb *uz)
{
	int i; 
	struct Stroki* ukr = uz->str;
	for (i = 0; i < uz->m; i++)
	{
		free((ukr++)->mat);
	}
	free(uz->str);
	uz->str = NULL;
}

void getInt(int *adr)
{
	int n;
	do
	{
		n = scanf_s("%d", adr, 4); 
		if (n == 0)
		{
			printf_s("Ошибка. Повторите ввод \n");
			scanf_s("%*[^\n]");
		}
	} while (n == 0);
}

void getIntPol(int *rom)
{
	int shut;
	do
	{
		shut = scanf_s("%d", rom, 4);
		if (shut == 0 || *rom < 0)
		{
			printf_s("Ошибка. Повторите ввод. \n");
			scanf_s("%*c"); //scanf_s("%*[^\n]"); 
			shut = 0;
		}
	} while (shut == 0);
} 

void obrabotka(struct Stolb *uk)
{
	
	int j = uk->m; 
	int chislo;
	struct Stroki* ukr = uk->str; 
	while (j-- > 0)
	{
		int nn = ukr->n;
		int *p1 = ukr->mat; 
		int *p2 = ukr->mat;
		while (nn-- >0)
		{
			chislo=*p1;
			if (ObrabotkaChisla(chislo)){
				*p2++ = *p1;
			}
			p1++;
		}
		ukr->n = p2 - ukr->mat;
		ukr->mat = (int*)realloc(ukr->mat, ukr->n*sizeof(int));
		ukr++;
	}
}
void CopyMatr(struct Stolb *from, struct Stolb *to)
{
	int nn = from->m; 
		to->m = from->m;
	to->str = ((struct Stroki*)calloc(from->m, sizeof(struct Stroki)));
	struct Stroki* ukr1 = from->str;
	struct Stroki* ukr2 = to->str; 
	while (nn--)
	{
		int *p1, *p2;
		ukr2->n = ukr1->n;
		ukr2->mat=((int*)calloc(to->str->n, sizeof(int*)));
		p1 = ukr1->mat;
		p2 = ukr2->mat;
		int nnn = ukr1->n;
		while (nnn-->0)
			*p2++ = *p1++;
		ukr1++; ukr2++;
	}
	
}

int ObrabotkaChisla(int chislo)
{
	int f, a, logic = 1;
	f = abs(chislo);
	do
	{
		a = f % 10;
		a = a & 1;
		f = f / 10;
		if (a != 0)
		{
			logic = 0;
		}
	} while (logic && f > 0);
	return logic;
}