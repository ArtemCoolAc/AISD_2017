#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>

typedef struct Item {
	char c;
	struct Item *next;
} Item;

Item *createList(const char *);
void putList(Item *);
Item *deleteList(Item *);
Item *SkipSpace(Item *);
Item *DelSpace(Item *);
Item *DeleteSymbols(Item * prev, char bukva);
char Symbol(Item *ptr);

Item *createList(const char *str)
{
	Item head = { '*', NULL };
	Item *last = &head;
	while (*str != '\0'){
		last->next = (Item *)malloc(sizeof(Item));
		last = last->next;
		last->c = *str++;
		last->next = NULL;
	}
	return head.next;
}

void putList(char *msg, Item *ptr)
{
	printf("%s: \"", msg);
	for (; ptr != NULL; ptr = ptr->next)
		printf("%c", ptr->c);
	printf("\"\n");
}

Item *deleteList(Item *ptr)
{
	Item *tmp = NULL;
	while (ptr != NULL){
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return ptr;
}

  Item *Obrabotka(Item *current)
{	
	  Item PreHead = { '*', current };
	  Item* ptr = &PreHead;
	  Item *last = NULL;
	  char c = Symbol(current);
	  if (!c)
		  return NULL;
	  while (ptr && (ptr->next = DelSpace(ptr->next))) {
		  last = DeleteSymbols(ptr, c);
		  if (last->c == ' ' || last->c == '\t')
			  ptr = last;
		  else
			  ptr = last->next;
		  if (ptr)
			  ptr->c = ' ';
	}
	  if (last->next != NULL) {
		  free(last->next);
		  last->next = NULL;
	  }
	  //PreHead.next = DelSpace(PreHead.next);
	  return PreHead.next;
}

 Item *DelSpace(Item *ptr)
{
	Item  *tmp;
	while (ptr && (ptr->c == ' ' ||ptr->c =='\t')) {
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return ptr;
} 

char Symbol(Item *ptr)
{
	Item * ptr1 = ptr;
	char s;
	while (ptr1 && ptr1->next && ((ptr1->c == ' ') || (ptr1->c =='\t')))
		ptr1 = ptr1->next;
	s = ptr1?ptr1->c:'\0';
	return s;
}

Item *DeleteSymbols(Item * prev, char bukva)
{
	Item *tmp; 
	while (prev && prev->next && ( (prev->next->c != ' ') || (prev->next->c != '\t'))) {
		if (prev->next->c == bukva) {
			tmp = prev->next;
			prev->next = tmp->next;
			free(tmp);
		}
		else
			prev = prev->next;
	}
	return prev;
}

int main()
{
	char buf[80];
	Item *st; char s;
	setlocale(LC_ALL, "Rus");
	SetConsoleCP(1251); // Ввод с консоли в кодировке 1251
	SetConsoleOutputCP(1251); // Вывод -.-
	while (puts("Введите строку"), gets_s(buf)){
		st = createList(buf);
		st = Obrabotka(st);
		putList("Обработанная строка", st);
		st = deleteList(st);
	}
	return 0;
}