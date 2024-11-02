#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strtoi.h"
#include "tut5.h"

#define BUFSIZE   (sizeof(unsigned int) * 8 + 1)
#define STR(x)    #x
#define FMTSTR(x) "%" STR(x) "s %d"

/* --- function prototypes -------------------------------------------------- */

void set_student(Item *p, char *name, int number);
void binary_sort_c(Item *list, int n);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{
	char name[NAME_LEN+1];
	int i, n, num;
	Item *students_asm, *students_c;

	/* check command-line arguments */
	if (argc != 2)
		goto usage;

	/* convert command-line arguments */
	if (strtoi(argv[1], &n) != EXIT_SUCCESS)
		goto usage;

	/* read data */
	students_asm = malloc(n * sizeof(Item));
	students_c = malloc(n * sizeof(Item));
	if (students_asm == NULL || students_c == NULL) {
		fprintf(stderr, "Memory allocation failed.\n");
		return EXIT_FAILURE;
	}

	for (i = 0; i < n; i++) {
		if (scanf(FMTSTR(NAME_LEN), name, &num) != 2) {
			fprintf(stderr, "Malformed field (%d)\n", i - 1);
			return EXIT_FAILURE;
		}
		set_student(&students_asm[i], name, num);
	}

	memcpy(students_c, students_asm, n * sizeof(Item));

	/* test */
	binary_sort_c(students_c, n);
	binary_sort(students_asm, n);

	printf("binary insertion sort of %d item(s): ", n);
	if (memcmp(students_asm, students_c, n * sizeof(Item)) == 0)
		printf("\033[32mcorrect\033[m\n");
	else {
		printf("\033[31mincorrect\033[m; the list is:\n");
		for (i = 0; i < n; i++)
			printf("- %d, %s\n", students_asm[i].number, students_asm[i].name);
		printf("and must be:\n");
		for (i = 0; i < n; i++)
			printf("- %d, %s\n", students_c[i].number, students_c[i].name);
	}

	return EXIT_SUCCESS;

usage:
	fprintf(stderr, "usage: %s <nrecords>\n", argv[0]);
	return EXIT_FAILURE;
}

/* --- library -------------------------------------------------------------- */

void set_student(Item *p, char *name, int number)
{
	if (p) {
		strcpy(p->name, name);
		p->number = number;
	}
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
