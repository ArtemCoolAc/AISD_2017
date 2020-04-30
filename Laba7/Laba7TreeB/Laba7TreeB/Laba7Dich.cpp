#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define TEST_MODE 1

typedef struct node {
	int n;
	int leaf = 0;
	int keys[3];
	char* info[3];
	node* next[4];
	node* prev;
} node;

typedef struct tab {
	int key = 0;
	int busy = 0;
	node* elem;
} tab;

typedef struct errors {
	const char* error;
	int fatal;
} errors;

int getInt(int *a);
int getStr(char** res);
int initialize(int* N, int* created, tab** table);
int load(node** root, char**loc, tab** table, int*N, int*created, int*h);
int addelem(node**root, int*h, tab*table, int N);
int findelem(node*root, tab* table, int N);
int output(node* root, int*n);
int outputtree(node* root);
int findmax(node* root);
int save(node** root, char**loc);
int saveas(node** root, char**loc);
node* findp(node * root, int key);
int add(int key, char*info, node**root, node**elem, int n, int* h, tab*table, int N);
int hk(int key, int N);
int cleartree(node**root);
int clearall(node**root, tab**table, char**loc, int* init);
int deleteelem(node* root, tab*table, int N);
int TestFill(node**root, int*h, tab**table, int* N, int* created);

int main() {
	const char* msgnew[]{
		"0: Exit",
		"1: Initialize tree",
		"2: Load from file" },
		*msgold[]{
		"0: Exit",
		"1: Add element",
		"2: Find element",
		"3: Delete element",
		"4: Output table",
		"5: Output table as tree",
		"6: Find max key",
		"7: Save table",
		"8: Save table as...",
		"9: Reinitialize" },
		*msgexit[]{
		"0: Back",
		"1: Don't save",
		"2: Save",
		"3: Save as..." };
	errors error[]{
		{ "Succeded", 0 },
		{ "End of file was occured", 1 },
		{ "Failed to allocate memory", 1 },
		{ "Aborting programm",1 },
		{ "Element with that key already exists",0 },
		{ "Element with that key does't exist",0 },
		{ "Tree is empty",0 },
		{ "Failed to open file",0 } };
	int err, N, h = 0;
	node* root = 0;
	tab* table = 0;
	int mode, created = 0;
	char*loc = 0;
	while (1) {
		switch (created)
		{
		case 0:
			for (int i = 0; i < 3; i++)
				printf("%s\n", msgnew[i]);
			do {
				err = getInt(&mode);
				if (err)
					break;
			} while (mode>2 || mode < -1 || (mode<0 && !TEST_MODE));
			switch (mode)
			{
			case -1:
				err = TestFill(&root, &h, &table, &N, &created);
				break;
			case 0:
				err = 3;
				break;
			case 1:
				err = initialize(&N, &created, &table);
				break;
			case 2:
				err = load(&root, &loc, &table, &N, &created, &h);
				break;
			default:
				break;
			}
			break;
		case 1:
			for (int i = 0; i < 10; i++)
				printf("%s\n", msgold[i]);
			do {
				err = getInt(&mode);
				if (err)
					break;
			} while (mode>9 || mode < 0);
			switch (mode)
			{
			case 0:
				for (int i = 0; i < 4; i++)
					printf("%s\n", msgexit[i]);
				do {
					err = getInt(&mode);
					if (err)
						break;
				} while (mode>3 || mode < 0);
				switch (mode)
				{
				case 0:
					err = 0;
					break;
				case 1:
					clearall(&root, &table, &loc, &created);
					err = 3;
					break;
				case 2:
					err = save(&root, &loc);
					if (!err) {
						clearall(&root, &table, &loc, &created);
						err = 3;
					}
					break;
				case 3:
					err = saveas(&root, &loc);
					if (!err) {
						clearall(&root, &table, &loc, &created);
						err = 3;
					}
					break;
				default:
					break;
				}
				break;
			case 1:
				err = addelem(&root, &h, table, N);
				break;
			case 2:
				err = findelem(root, table, N);
				break;
			case 3:
				err = deleteelem(root, table, N);
				break;
			case 4:
				err = output(root, 0);
				break;
			case 5:
				err = outputtree(root);
				break;
			case 6:
				err = findmax(root);
				break;
			case 7:
				err = save(&root, &loc);
				break;
			case 8:
				err = saveas(&root, &loc);
				break;
			case 9:
				err = clearall(&root, &table, &loc, &created);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		if (err) {
			printf("%s\n", error[err].error);
			system("pause>>void");
			if (error[err].fatal)
				return 0;
		}
	}
	return 0;
}

int getInt(int *a)
{
	int n;
	do {
		n = scanf_s("%d", a, sizeof(int));
		if (n < 0)
			return 1;
		if (n == 0) {
			printf("%s\n", "Error! Repeat input");
			scanf_s("%*c", 0);
		}
	} while (n == 0);
	return 0;
}

int getStr(char** res)
{
	char buf[21];
	int n, len = 0, curLen;
	char *ptr = (char *)malloc(1);
	if (!ptr)
		return 2;
	*ptr = '\0';

	do {
		n = scanf_s("%20[^\n]s", buf, 21);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			return 1;
		}
		if (n > 0) {
			curLen = strlen(buf);
			len += curLen;
			ptr = (char *)realloc(ptr, len + 1);
			if (!ptr)
				return 2;
			strcat_s(ptr, len + 1, buf);
		}
		else
			scanf_s("%*c", 0);
	} while ((n > 0) || (ptr[0] == '\0'));
	*res = ptr;
	return 0;
}

int initialize(int* N, int* created, tab** table) {
	int err;
	printf("\nInput amount of lines: ");
	do {
		err = getInt(N);
		if (err)
			return err;
	} while (*N < 1);
	*table = (tab*)malloc(*N * sizeof(tab));
	if (!*table)
		return 2;
	*created = 1;
	printf("\nTable initialized\n");
	system("pause>>void");
	return 0;
}

int loadelem(FILE*file, node** root, int*h, tab*table, int N) {
	int key, n;
	char* buf;
	node*elem = *root;
	fread_s(&key, sizeof(int), sizeof(int), 1, file);
	fread_s(&n, sizeof(int), sizeof(int), 1, file);
	buf = (char*)malloc(sizeof(char)*n);
	fread_s(buf, sizeof(char)*n, sizeof(char), n, file);
	add(key, buf, root, &elem, *h, h, table, N);
	return 0;
}

int load(node** root, char**loc, tab** table, int*N, int*created, int*h) {
	int err, n;
	FILE *file;
	unsigned long long t;
	printf("\nInput amount of lines: ");
	do {
		err = getInt(N);
		if (err)
			return err;
	} while (*N < 1);
	*table = (tab*)malloc(*N * sizeof(tab));
	if (!*table)
		return 2;
	*created = 1;
	printf("\nTable initialized\n");
	if (!*loc) {
		printf("Input file location\n");
		err = getStr(loc);
		if (err)
			return err;
	}
	if (TEST_MODE)
		t = GetTickCount64();
	fopen_s(&file, *loc, "rb");
	if (!file)
		return 7;
	fread_s(&n, sizeof(int), sizeof(int), 1, file);
	for (int i = 0; i < n; i++)
		loadelem(file, root, h, *table, *N);
	fclose(file);
	if (TEST_MODE)
		t = GetTickCount64() - t;
	*created = 1;
	printf("\nData loaded\n");
	if (TEST_MODE) {
		if (t == 0)
			printf("Loading took less than 16 millisecond\n");
		else
			printf("Loading took about %ld milliseconds\n", t);
	}
	system("pause>>void");
	return 0;
}

int addelem(node**root, int*h, tab*table, int N) {
	int key, err, way = 0;
	char* info;
	node*elem = *root;
	unsigned long *t;
	printf("\nInput key: ");
	err = getInt(&key);
	if (err)
		return err;

	if (table[hk(key, N)].busy)
		if (table[hk(key, N)].key == key)
			return 4;
	printf("\nInput info: ");
	err = getStr(&info);
	if (err)
		return err;
	if (TEST_MODE) {
		t = (unsigned long*)malloc(sizeof(unsigned long));
		if (t)
			*t = GetTickCount();
	}
	err = add(key, info, root, &elem, *h, h, table, N);
	if (err)
		return err;
	if (TEST_MODE&&t)
		*t = GetTickCount() - *t;
	printf("\nElement appended\n");
	if (TEST_MODE&&t) {
		if (*t == 0)
			printf("Adding element took less than 16 millisecond\n");
		else
			printf("Adding element took about %ld milliseconds\n", *t);
		free(t);
	}
	system("pause>>void");
	return 0;
}

int findelem(node*root, tab* table, int N) {
	int err, key, hkey;
	node*sb;
	unsigned long *t;
	if (!root)
		return 6;
	printf("\nInput key: ");
	err = getInt(&key);
	if (err)
		return err;
	hkey = hk(key, N);
	if (TEST_MODE) {
		t = (unsigned long*)malloc(sizeof(unsigned long));
		if (t)
			*t = GetTickCount();
	}
	if (table[hkey].busy) {
		if (table[hkey].key == key) {
			for (int i = 0; i < table[hkey].elem->n; i++) {
				if (table[hkey].elem->keys[i] == key) {
					if (TEST_MODE&&t)
						*t = GetTickCount() - *t;
					printf("\nInformation: %s\n", table[hkey].elem->info[i]);
					if (TEST_MODE&&t) {
						if (*t == 0)
							printf("Searching took less than 16 millisecond, data was found in the table\n");
						else
							printf("Searching took about %ld milliseconds, data was found in the table\n", *t);
						free(t);
					}
					system("pause>>void");
					return 0;
				}
			}
		}
	}
	sb = findp(root, key);
	if (sb == 0)
		return 5;
	for (int i = 0; i < sb->n; i++) {
		if (sb->keys[i] == key&&sb->info[i]) {
			if (TEST_MODE&&t)
				*t = GetTickCount() - *t;
			printf("\nInformation: %s\n", sb->info[i]);
			if (TEST_MODE&&t) {
				if (*t == 0)
					printf("Searching took less than 16 millisecond, data was found in the tree\n");
				else
					printf("Searching took about %ld milliseconds, data was found in the tree\n", *t);
				free(t);
			}
			system("pause>>void");
			return 0;
		}
	}
	return 5;
}

int output(node* root, int*n) {
	int in = 0;
	unsigned long *t = 0;
	if (!root)
		return 6;
	if (!n) {
		n = (int*)malloc(sizeof(int));
		in = 1;
		if (!n)
			return 2;
		if (TEST_MODE) {
			t = (unsigned long*)malloc(sizeof(unsigned long));
			if (t)
				*t = GetTickCount();
		}
		*n = 1;
	}

	if (root->leaf) {
		for (int i = 0; i < root->n; i++) {
			if (root->info[i]) {
				printf("%d key: %d, info: %s\n", *n, root->keys[i], root->info[i]);
				(*n)++;
			}
		}
	}
	else {
		for (int i = 0; i < root->n + 1; i++) {
			output(root->next[i], n);
			if (i < root->n&&root->info[i]) {
				printf("%d key: %d, info: %s\n", *n, root->keys[i], root->info[i]);
				(*n)++;
			}
		}
	}
	if (in) {
		if (*n == 1) {
			free(n);
			return 6;
		}
		free(n);
		printf("\n");
		if (TEST_MODE&&t)
			*t = GetTickCount() - *t;
		if (TEST_MODE&&t) {
			if (*t == 0)
				printf("Outputing tree took less than 16 millisecond\n");
			else
				printf("Outputing tree took about %ld milliseconds\n", *t);
			free(t);
		}
		system("pause>>void");
	}
	return 0;
}

void drawelem(node* root) {
	int l = 0, b;
	for (int i = 0; i < root->n; i++) {
		b = root->keys[i];
		if (!b)
			l++;
		while (b > 0) {
			l++;
			b /= 10;
		}
		l++;
	}
	l += 3;

	for (int i = 0; i < l / 2 + 1; i++)
		printf(" ");
	printf("0\n");
	for (int i = 0; i < l / 2 + 1; i++)
		printf(" ");
	printf("|\n ");
	for (int i = 0; i < l; i++)
		printf("*");
	if (root->n >= 1)
		printf("     Key: %d, Info: %s", root->keys[0], root->info[0] ? root->info[0] : "- Deleted");
	printf("\n * ");
	for (int i = 0; i < root->n; i++)
		printf("%d ", root->keys[i]);
	printf("*");
	if (root->n >= 2)
		printf("     Key: %d, Info: %s", root->keys[1], root->info[1] ? root->info[1] : "- Deleted");
	printf("\n ");
	for (int i = 0; i < l; i++)
		printf("*");
	if (root->n >= 3)
		printf("     Key: %d, Info: %s", root->keys[2], root->info[2] ? root->info[2] : "- Deleted");
	printf("\n");
	if (!root->leaf) {
		printf("  |");
		for (int i = 0; i < root->n; i++) {
			b = root->keys[i];
			do {
				printf(" ");
				b /= 10;
			} while (b > 0);
			printf("|");
		}
		printf("\n  1");
		for (int i = 0; i < root->n; i++) {
			b = root->keys[i];
			do {
				printf(" ");
				b /= 10;
			} while (b > 0);
			printf("%d", i + 2);
		}
	}
	printf("\n");
	return;
}

int outputtree(node* root) {
	int mode, err;
	if (!root)
		return 6;
	while (1) {
		drawelem(root);
		do {
			err = getInt(&mode);
			if (err)
				return err;
		} while (mode > (root->n + 2) || mode < 0 || (mode>0 && root->leaf));
		printf("\n");
		if (mode == 0) {
			return 0;
		}
		else {
			outputtree(root->next[mode - 1]);
		}
	}
	return 0;
}

int findmax(node* root) {
	if (!root)
		return 6;
	if (!root->leaf) {
		if (!findmax(root->next[root->n]))
			return 0;
		for (int i = root->n - 1; i >= 0; i--) {
			if (root->info[i]) {
				printf("\nMaximum key: %d ,Info: %s\n", root->keys[i], root->info[i]);
				return 0;
			}
		}
		return 6;
	}
	if (root->leaf) {
		for (int i = root->n - 1; i >= 0; i--) {
			if (root->info[i]) {
				printf("\nMaximum key: %d ,Info: %s\n\n", root->keys[i], root->info[i]);
				system("pause>>void");
				return 0;
			}
		}
		return 6;
	}
	return 0;
}

int sav(node* elem, FILE* file, int*n) {
	int l;
	if (!elem->leaf) {
		for (int i = 0; i <= elem->n; i++) {
			sav(elem->next[i], file, n);
		}
	}
	for (int i = 0; i < elem->n; i++) {
		if (elem->info[i]) {
			fwrite(&elem->keys[i], sizeof(int), 1, file);
			l = strlen(elem->info[i]) + 1;
			fwrite(&l, sizeof(int), 1, file);
			fwrite(elem->info[i], sizeof(char), l, file);
			(*n)++;
		}
	}
	return 0;
}

int save(node** root, char**loc) {
	int err, n = 0;
	FILE *file = 0;
	unsigned long *t;
	if (!*root)
		return 6;
	if (!*loc) {
		printf("Input file location\n");
		err = getStr(loc);
		if (err)
			return err;
	}
	if (TEST_MODE) {
		t = (unsigned long*)malloc(sizeof(unsigned long));
		if (t)
			*t = GetTickCount();
	}
	fopen_s(&file, *loc, "wb");
	if (!file)
		return 7;
	fwrite(&n, sizeof(int), 1, file);
	sav(*root, file, &n);
	fseek(file, 0, SEEK_SET);
	fwrite(&n, sizeof(int), 1, file);
	fclose(file);
	if (TEST_MODE&&t)
		*t = GetTickCount() - *t;
	printf("\nData saved\n");
	if (TEST_MODE&&t) {
		if (*t == 0)
			printf("Saving data took less than 16 millisecond\n");
		else
			printf("Saving data took about %ld milliseconds\n", *t);
		free(t);
	}
	system("pause>>void");
	return 0;
}

int saveas(node** root, char**loc) {
	int err, n = 0;
	FILE *file = 0;
	unsigned long *t;
	if (!*root)
		return 6;
	printf("Input file location\n");
	err = getStr(loc);
	if (err)
		return err;
	if (TEST_MODE) {
		t = (unsigned long*)malloc(sizeof(unsigned long));
		if (t)
			*t = GetTickCount();
	}
	fopen_s(&file, *loc, "wb");
	if (!file)
		return 7;
	fwrite(&n, sizeof(int), 1, file);
	sav(*root, file, &n);
	fseek(file, 0, SEEK_SET);
	fwrite(&n, sizeof(int), 1, file);
	fclose(file);
	if (TEST_MODE&&t)
		*t = GetTickCount() - *t;
	printf("\nData saved\n");
	if (TEST_MODE&&t) {
		if (*t == 0)
			printf("Saving data took less than 16 millisecond\n");
		else
			printf("Saving data tree took about %ld milliseconds\n", *t);
		free(t);
	}
	system("pause>>void");
	return 0;
}

node* findp(node * root, int key) {
	int way = 0;
	if (!root)
		return 0;
	for (int i = 0; i < root->n; i++) {
		if (root->keys[i] == key)
			return root;
	}
	if (!root->leaf) {
		for (int i = 0; (i < root->n) && key>root->keys[i]; i++) {
			way++;
		}
		return findp(root->next[way], key);
	}
	return 0;
}

int separate(node**root, node*elem, int*h, tab*table, int N) {
	int buf, way, err;
	char*bufch;
	node *nodebufl, *nodebufr;

	buf = elem->keys[1];
	bufch = elem->info[1];
	nodebufl = elem;
	nodebufr = (node*)malloc(sizeof(node));
	if (!nodebufr)
		return 2;
	nodebufl->n = 1;
	nodebufr->n = 1;
	nodebufr->leaf = nodebufl->leaf;
	nodebufr->keys[0] = nodebufl->keys[2];
	nodebufr->info[0] = nodebufl->info[2];
	nodebufr->next[0] = nodebufl->next[2];
	nodebufr->next[1] = nodebufl->next[3];
	if (!elem->prev) {
		elem->prev = (node*)malloc(sizeof(node));
		if (!elem->prev)
			return 2;

		elem->prev->n = 0;
		elem->prev->leaf = 0;
		elem->prev->prev = 0;
		(*h)++;
		*root = elem->prev;
	}
	if (elem->prev->n >= 3) {
		err = separate(root, elem->prev, h, table, N);
		if (err)
			return err;
	}
	way = 0;
	for (int i = 0; i < elem->prev->n && elem->prev->keys[way] < buf; i++)
		way++;
	for (int i = elem->prev->n; i > way; i--) {
		elem->prev->keys[i] = elem->prev->keys[i - 1];
		elem->prev->info[i] = elem->prev->info[i - 1];
	}
	for (int i = elem->prev->n + 1; i > way; i--) {
		elem->prev->next[i] = elem->prev->next[i - 1];
	}
	elem->prev->keys[way] = buf;
	elem->prev->info[way] = bufch;
	elem->prev->next[way] = nodebufl;
	elem->prev->next[way + 1] = nodebufr;
	elem->prev->n++;
	nodebufl->prev = elem->prev;
	nodebufr->prev = elem->prev;
	if (!nodebufr->leaf) {
		for (int i = 0; i < nodebufr->n + 1; i++)
			nodebufr->next[i]->prev = nodebufr;
	}
	table[hk(nodebufl->keys[0], N)].elem = nodebufl;
	table[hk(nodebufr->keys[0], N)].elem = nodebufr;
	table[hk(buf, N)].elem = elem->prev;
	return 0;
}

int add(int key, char*info, node**root, node**elem, int n, int* h, tab*table, int N) {
	int way = 0, err, buf;
	node* nodebufl, *nodebufr;
	char* bufch;
	if (*elem == 0) {
		*elem = (node*)malloc(sizeof(node));
		if (!*elem)
			return 2;
		(*elem)->n = 1;
		(*elem)->keys[0] = key;
		(*elem)->info[0] = info;
		(*elem)->prev = 0;
		(*elem)->leaf = 1;
		(*h)++;
		*root = *elem;
		table[hk(key, N)].key = key;
		table[hk(key, N)].elem = *elem;
		table[hk(key, N)].busy = 1;
		return 0;
	}
	way = 0;
	if ((*elem)->leaf) {
		if ((*elem)->n >= 3) {
			err = separate(root, *elem, h, table, N);
			if (err)
				return err;
			err = add(key, info, root, &(*elem)->prev, n, h, table, N);
			return err;
		}
		for (int i = 0; i < (*elem)->n && (*elem)->keys[way] < key; i++)
			way++;
		if ((*elem)->keys[way] == key) {
			if ((*elem)->info[way])
				return 4;
			else {
				(*elem)->info[way] = info;
				return 0;
			}
		}
		for (int i = (*elem)->n; i > way; i--) {
			(*elem)->keys[i] = (*elem)->keys[i - 1];
			(*elem)->info[i] = (*elem)->info[i - 1];
		}
		(*elem)->keys[way] = key;
		(*elem)->info[way] = info;
		(*elem)->n++;
		table[hk(key, N)].key = key;
		table[hk(key, N)].elem = *elem;
		table[hk(key, N)].busy = 1;
		return 0;
	}
	for (int i = 0; i < (*elem)->n && (*elem)->keys[way] < key; i++)
		way++;
	if ((*elem)->keys[way] == key) {
		if ((*elem)->info[way])
			return 4;
		else {
			(*elem)->info[way] = info;
			return 0;
		}
	}
	err = add(key, info, root, &(*elem)->next[way], n - 1, h, table, N);
	return err;
}

int hk(int key, int N) {
	return key%N;
}

int cleartree(node**root) {
	if (!(*root)->leaf) {
		for (int i = 0; i <= (*root)->n; i++) {
			cleartree(&(*root)->next[i]);
		}
	}
	for (int i = 0; i < (*root)->n; i++) {
		if ((*root)->info)
			free((*root)->info[i]);
	}
	free(*root);
	*root = 0;
	return 0;
}

int clearall(node**root, tab**table, char**loc, int* init) {
	unsigned long *t;
	if (TEST_MODE) {
		t = (unsigned long*)malloc(sizeof(unsigned long));
		if (t)
			*t = GetTickCount();
	}
	if (*root)
		cleartree(root);
	free(*table);
	*table = 0;
	if (*loc)
		free(*loc);
	*loc = 0;
	*init = 0;
	if (TEST_MODE&&t)
		*t = GetTickCount() - *t;
	printf("\nTree is cleared\n");
	if (TEST_MODE&&t) {
		if (*t == 0)
			printf("Clearing took less than 16 millisecond\n");
		else
			printf("Clearing took about %ld milliseconds\n", t);
		free(t);
	}
	system("pause>>void");
	return 0;
}

int deleteelem(node* root, tab*table, int N) {
	int err, key, way = 0;
	node*buf = 0;
	unsigned long *t;
	printf("\nInput key: ");
	err = getInt(&key);
	if (err)
		return err;
	if (TEST_MODE) {
		t = (unsigned long*)malloc(sizeof(unsigned long));
		if (t)
			*t = GetTickCount();
	}
	way = hk(key, N);
	if (table[way].key == key) {
		buf = table[way].elem;
	}
	way = 0;
	if (!buf)
		buf = findp(root, key);
	if (!buf)
		return 5;
	way = 0;
	for (int i = 0; i < buf->n&&buf->keys[way] != key; i++) {
		way++;
	}
	if (!buf->info[way])
		return 5;
	free(buf->info[way]);
	buf->info[way] = 0;
	way = hk(key, N);
	if (table[way].key == key) {
		table[way].busy = 0;
	}
	if (TEST_MODE&&t)
		*t = GetTickCount64() - *t;
	printf("\nElement deleted\n");
	if (TEST_MODE&&t) {
		if (*t == 0)
			printf("Deleting element took less than 16 millisecond\n");
		else
			printf("Deleting element tree took about %ld milliseconds\n", t);
		free(t);
	}
	system("pause>>void");
	return 0;
}

int TestFill(node**root, int*h, tab**table, int* N, int* created) {
	char*tmp;
	node*elem;
	int n, err;
	unsigned long t;
	printf("\nInput amount of lines: ");
	do {
		err = getInt(N);
		if (err)
			return err;
	} while (*N < 1);
	*table = (tab*)malloc(*N * sizeof(tab));
	if (!*table)
		return 2;
	*created = 1;
	printf("\nTable initialized\n");
	printf("\nInput amount of elements: ");
	do {
		err = getInt(&n);
		if (err)
			return err;
	} while (n < 0);
	t = GetTickCount();
	for (int i = 0; i < n; i++) {
		tmp = (char*)malloc(sizeof(char) * 5);
		tmp[0] = 'I'; tmp[1] = 'n'; tmp[2] = 'f'; tmp[3] = 'o'; tmp[4] = '\0';
		elem = *root;
		err = add(i, tmp, root, &elem, *h, h, *table, *N);
		if (err)
			return err;
	}
	t = GetTickCount() - t;
	if (t == 0)
		printf("Table was filled in less than 16 millisecond\n");
	else
		printf("Table was filled in about %ld milliseconds\n", t);
	system("pause>>void");
}

