#ifndef STRTOI_H
#define STRTOI_H

/**
 * Converts a string that represents a decimal numeric literal into an
 * <code>int</code>.  Note that this function outputs an error message to the
 * standard error stream if the string literal represents a number that does not
 * fit into an <code>int</code> or is not a valid representation of a decimal
 * integer.
 *
 * @param[in]   str
 *     the string representation of a decimal numeric literal
 * @param[out]  num
 *     the integer specified by the string
 *
 * @return <code>EXIT_SUCCESS</code> if the conversion was successful;
 *         otherwise, <code>EXIT_FAILURE</code>
 */
int strtoi(const char *str, int *num);

#endif /* STRTOI_H */
