#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int strtoi(const char *str, int *num)
{
	char *end;
	long lnum;

	errno = 0;
	lnum = strtol(str, &end, 10);

	/* Note: Library functions that output error messages directly are typically
	 * viewed as bad design... each of the following cases should rather have
	 * just returned a different error code.  However, since this function is
	 * meant for use in a setup for testing, allowing output here makes the
	 * testing itself more succinct.
	 */
	if (lnum > INT_MAX || (errno == ERANGE && lnum == LONG_MAX)) {
		fprintf(stderr, "int overflow for decimal literal '%s'\n", str);
		return EXIT_FAILURE;
	}
	if (lnum < INT_MIN || (errno == ERANGE && lnum == LONG_MIN)) {
		fprintf(stderr, "int underflow for decimal literal '%s'\n", str);
		return EXIT_FAILURE;
	}
	if (*end != '\0') {
		fprintf(stderr, "unsupported decimal int literal '%s'\n", str);
		return EXIT_FAILURE;
	}

	*num = (int) lnum;
	return EXIT_SUCCESS;
}
