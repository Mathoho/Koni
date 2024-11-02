#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strtoi.h"
#include "tut5.h"

#define BUFSIZE (sizeof(unsigned int) * 8 + 1)

/* --- function prototypes -------------------------------------------------- */

void bin_to_string_c(int n, char *s);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{
	char s_asm[BUFSIZE], s_c[BUFSIZE];
	int n;

	/* check command-line arguments */
	if (argc != 2)
		goto usage;

	/* convert command-line arguments */
	if (strtoi(argv[1], &n) != EXIT_SUCCESS)
		goto usage;

	/* test */
	bin_to_string_c(n, s_c);
	bin_to_string(n, s_asm);

	printf("power(n=%d, s) -> s=%s; ", n, s_asm);
	if (strcmp(s_c, s_asm) == 0)
		printf("\033[32mcorrect\033[m\n");
	else
		printf("\033[31mincorrect\033[m; should have s=%s\n", s_c);

	return EXIT_SUCCESS;

usage:
	fprintf(stderr, "usage: %s <int>\n", argv[0]);
	return EXIT_FAILURE;
}

/* --- library -------------------------------------------------------------- */

void bin_to_string_c(int n, char *s)
{
	unsigned int i, x;

	for (i = 0, x = 0x80000000; i < sizeof(unsigned int) * 8; i++, x >>= 1)
		s[i] = (n & x) ? '1' : '0';
	s[i] = '\0';
}
