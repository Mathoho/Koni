#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tut5.h"

#define MAX_STUDENTS_A 1
#define MAX_STUDENTS_B 2
#define MAX_STUDENTS_C 14
#define MAX_BIN_TO_STRING_VALUES 10

void create_student(Item *p, char *name, int number)
{
	if (p) {
		strcpy(p->name, name);
		p->number = number;
	}
}

void display_students(Item list[], int n)
{
	int i;

	for (i = 0; i < n; i++)
		printf("%d, %s\n", list[i].number, list[i].name);
}

void binary_sort_c(Item *list, int n)
{
	int i, j, bottom, top, middle;
	Item temp;

	if (list) {
		for (i = 1; i < n; i++) {
			temp = list[i];
			bottom = 0;
			top = i - 1;
			while (bottom <= top) {
				middle = bottom + (top - bottom) / 2;
				if (temp.number < list[middle].number)
					top = middle - 1;
				else
					bottom = middle + 1;
			}
			for (j = i - 1; j >= bottom; j--)
				list[j+1] = list[j];
			list[bottom] = temp;
		}
	}
}


void enumerate(const Node *root)
{
	if (root) {
		enumerate(root->left);
		printf("%s\n", root->name);
		enumerate(root->right);
	}
}

void insert_node(Node **root, Node *n)
{
	if (*root == NULL)
		*root = n;
	else if (strcmp(n->name, (*root)->name) > 0)
		insert_node(&(*root)->right, n);
	else if (strcmp(n->name, (*root)->name) < 0)
		insert_node(&(*root)->left, n);
}

int equal(Node *r1, Node *r2)
{
	if ((r1 == NULL) && (r2 == NULL))
		return 1;
	else
		return ((r1 != NULL) && (r2 != NULL) &&
			(strcmp(r1->name, r2->name) == 0) &&
			equal(r1->right, r2->right) && equal(r1->left, r2->left));
}

Node* create_node(char *name)
{
	Node* n;

	n = malloc(sizeof(Node)); // XXX: assume non-NULL address
	strcpy(n->name, name);
	n->left = NULL;
	n->right = NULL;

	return n;
}

void remove_max_c(Node **root, Node **max)
{
	if ((*root)->right)
		remove_max_c(&(*root)->right, max);
	else {
		*max = *root;
		*root = (*max)->left;
	}
}

void delete_node_c(Node **root, char *name)
{
	Node *n, *temp;

	if (*root) {
		if (strcmp(name, (*root)->name) < 0)
			delete_node_c(&(*root)->left, name);
		else if (strcmp(name, (*root)->name) > 0) {
			delete_node_c(&(*root)->right, name);
		} else {
			temp = *root;
			if ((*root)->left == NULL)
				*root = (*root)->right;
			else if ((*root)->right == NULL)
				*root = (*root)->left;
			else {
				remove_max_c(&(*root)->left, &n);
				n->left = (*root)->left;
				n->right = (*root)->right;
				*root = n;
			}
			free(temp);
		}
	}
}

void bin_to_string_c(int n, char *s)
{
	int i;
	unsigned int x;

	for (i = 0, x = 0x80000000; i < sizeof(unsigned int) * 8; i++, x >>= 1)
		s[i] = (n & x) ? '1' : '0';
	s[i] = '\0';
}

void evaluate_binary_sort(char *msg, Item *l1, Item *l2, int n)
{
	printf("evaluating binary sort for %s -- ", msg);
	binary_sort(l1, n);
	binary_sort_c(l2, n);
	if (memcmp(l1, l2, n * sizeof(Item)) == 0)
		printf("\033[32mcorrect\033[m\n");
	else {
		printf("\033[31mincorrect\033[m -- the list contains:\n");
		display_students(l1, n);
		printf("and should contain:\n");
		display_students(l2, n);
	}
}

void evaluate_delete_node(Node *root_1, Node *root_2, char *name)
{
	static int count = 0;

	delete_node_c(&root_1, name);
	delete_node(&root_2, name);

	count++;
	if (equal(root_1, root_2) == 1)
		printf("delete_node %d -- \033[32mcorrect\033[m\n", count);
	else {
		printf("delete_node %d -- \033[31mincorrect\033[m -- binary tree",
				count);
		if (root_2) {
			printf(" contains\n");
			enumerate(root_2);
		} else
			printf(" is NULL\n");
		if (root_1) {
			printf("and should contain\n");
			enumerate(root_1);
		} else
			printf("and should be NULL\n");
	}
}

void evaluate_bin_to_string(int n)
{
	char s1[33], s2[33];

	bin_to_string_c(n, s1);
	bin_to_string(n, s2);

	printf("bin_to_string(%d) returned '%s' -- ", n, s2);
	if (strcmp(s1, s2) == 0)
		printf("\033[32mcorrect\033[m\n");
	else
		printf("\033[31mincorrect\033[m -- should return '%s'\n", s1);
}

int main(int argc, char *argv[])
{
	Item students_a[MAX_STUDENTS_A];
	Item students_a1[MAX_STUDENTS_A];
	Item students_b[MAX_STUDENTS_B];
	Item students_b1[MAX_STUDENTS_B];
	Item students_c[MAX_STUDENTS_C];
	Item students_c1[MAX_STUDENTS_C];

	int x[MAX_BIN_TO_STRING_VALUES];
	int i;

	Node* root_a;
	Node* root_a1;
	Node* root_b;
	Node* root_b1;
	Node* root_c;
	Node* root_c1;
	Node* root_d;
	Node* root_d1;
	Node* root_e;
	Node* root_e1;
	Node* root_f;
	Node* root_f1;
	Node* root_g;
	Node* root_g1;

	/* Binary sort test cases */
	create_student(&students_a[0], "Sam", 1387689);
	memcpy(students_a1, students_a, sizeof(Item) * MAX_STUDENTS_A);

	create_student(&students_b[0], "Marina", 1389735);
	create_student(&students_b[1], "Abraham", 1382024);
	memcpy(students_b1, students_b, sizeof(Item) * MAX_STUDENTS_B);

	create_student(&students_c[0], "Sam", 1387689);
	create_student(&students_c[1], "Marina", 1389735);
	create_student(&students_c[2], "Jacques", 1392755);
	create_student(&students_c[3], "Pietie", 1357987);
	create_student(&students_c[4], "Siphiwe", 1384426);
	create_student(&students_c[5], "Rita", 1387551);
	create_student(&students_c[6], "Thulani", 1389699);
	create_student(&students_c[7], "Linda", 1383689);
	create_student(&students_c[8], "Abongile", 1353339);
	create_student(&students_c[9], "Heinrich", 1382376);
	create_student(&students_c[10], "Abraham", 1382024);
	create_student(&students_c[11], "Will", 1387009);
	create_student(&students_c[12], "Ayanda", 1381987);
	create_student(&students_c[13], "Stefan", 1368754);
	memcpy(students_c1, students_c, sizeof(Item) * MAX_STUDENTS_C);

	printf("\033[97m***** results for 'binary_sort' *****\033[m\n");
	evaluate_binary_sort("students_a", students_a, students_a1, MAX_STUDENTS_A);
	evaluate_binary_sort("students_b", students_b, students_b1, MAX_STUDENTS_B);
	evaluate_binary_sort("students_c", students_c, students_c1, MAX_STUDENTS_C);

	/* node deletion from binary tree test cases */
	root_a = NULL;
	root_a1 = NULL;
	root_b = NULL;
	root_b1 = NULL;
	root_c = NULL;
	root_c1 = NULL;
	root_d = NULL;
	root_d1 = NULL;
	root_e = NULL;
	root_e1 = NULL;
	root_f = NULL;
	root_f1 = NULL;
	root_g = NULL;
	root_g1 = NULL;

	printf("\033[97m***** results for 'delete_node' *****\033[m\n");
	insert_node(&root_a, create_node("Cape Town"));
	insert_node(&root_a1, create_node("Cape Town"));
	evaluate_delete_node(root_a, root_a1, "Cape Town");

	insert_node(&root_b, create_node("Cape Town"));
	insert_node(&root_b, create_node("Johannesburg"));
	insert_node(&root_b1, create_node("Cape Town"));
	insert_node(&root_b1, create_node("Johannesburg"));
	evaluate_delete_node(root_b, root_b1, "Cape Town");

	insert_node(&root_c, create_node("Cape Town"));
	insert_node(&root_c, create_node("Johannesburg"));
	insert_node(&root_c1, create_node("Cape Town"));
	insert_node(&root_c1, create_node("Johannesburg"));
	evaluate_delete_node(root_c, root_c1, "Johannesburg");

	insert_node(&root_d, create_node("Cape Town"));
	insert_node(&root_d, create_node("Bloemfontein"));
	insert_node(&root_d1, create_node("Cape Town"));
	insert_node(&root_d1, create_node("Bloemfontein"));
	evaluate_delete_node(root_d, root_d1, "Cape Town");

	insert_node(&root_e, create_node("Cape Town"));
	insert_node(&root_e, create_node("Bloemfontein"));
	insert_node(&root_e1, create_node("Cape Town"));
	insert_node(&root_e1, create_node("Bloemfontein"));
	evaluate_delete_node(root_e, root_e1, "Bloemfontein");

	insert_node(&root_f, create_node("Cape Town"));
	insert_node(&root_f, create_node("Bloemfontein"));
	insert_node(&root_f, create_node("Johannesburg"));
	insert_node(&root_f1, create_node("Cape Town"));
	insert_node(&root_f1, create_node("Bloemfontein"));
	insert_node(&root_f1, create_node("Johannesburg"));
	evaluate_delete_node(root_f, root_f1, "Cape Town");

	insert_node(&root_g, create_node("Cape Town"));
	insert_node(&root_g, create_node("Bloemfontein"));
	insert_node(&root_g, create_node("Johannesburg"));
	insert_node(&root_g, create_node("Durban"));
	insert_node(&root_g, create_node("Bhisho"));
	insert_node(&root_g1, create_node("Cape Town"));
	insert_node(&root_g1, create_node("Bloemfontein"));
	insert_node(&root_g1, create_node("Johannesburg"));
	insert_node(&root_g1, create_node("Durban"));
	insert_node(&root_g1, create_node("Bhisho"));
	evaluate_delete_node(root_g, root_g1, "Bloemfontein");

	/* binary to string conversion test cases */
	x[0] = 0;
	x[1] = 1;
	x[2] = 16;
	x[3] = -1;
	x[4] = 123;
	x[5] = INT_MAX;
	x[6] = INT_MIN;
	x[7] = 1000;
	x[8] = 3;
	x[9] = 2;
	printf("\033[97m***** results for 'bin_to_string' *****\033[m\n");
	for (i = 0; i < MAX_BIN_TO_STRING_VALUES; i++) {
		evaluate_bin_to_string(x[i]);
	}

	return EXIT_SUCCESS;
}
