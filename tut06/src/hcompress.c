#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "heap.h"
#include "huffman.h"

#define BUF_SIZE 1024

/* --- function prototypes -------------------------------------------------- */

void huffman_delete_tree(HeapNode *root);
void charfreq(int fd, int freq[]);
void showfreq(int table[]);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{
	char *in_name, *out_name;
	int bi, ci, j;
	int ftable[MAX_HEAP_SIZE];
	int in_fd, out_fd;
	int nchars, tchars;
	size_t nbytes;
	unsigned char ch, b;
	unsigned char inbuf[BUF_SIZE], outbuf[BUF_SIZE];
	unsigned int i;
	Heap H;
	HeapNode hnode, *htree;
	HuffmanNode htable[MAX_HEAP_SIZE];

	if (argc != 2) {
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	in_name = argv[1];
	if ((out_name = malloc((strlen(in_name) + 4) * sizeof(char))) == NULL) {
		fprintf(stderr, "Memory allocation failed (out_name)\n");
		exit(EXIT_FAILURE);
	}
	strcpy(out_name, in_name);
	strcat(out_name, ".hz");

	if ((in_fd = open(in_name, O_RDONLY)) < 0) {
		fprintf(stderr, "File \"%s\" could not be opened: %s\n",
				in_name, strerror(errno));
		exit(EXIT_FAILURE);
	}

	charfreq(in_fd, ftable);
#ifdef DEBUG
	showfreq(ftable);
#endif
	close(in_fd);

	/* heap construction */
	heap_initialize(&H);
	for (i = 0; i < MAX_HEAP_SIZE; i++)
		if (ftable[i] > 0) {
			hnode.c = (unsigned char) i;
			hnode.frequency = ftable[i];
			hnode.left = hnode.right = NULL;
			heap_insert(&H, &hnode);
		}
	nchars = H.n;
	
	/* huffman table construction */
	huffman_build_tree(&H, &htree);
	huffman_initialize_table(htable);
	huffman_build_table(htree, htable, 0, 0);

	/* open files for processing */
	if ((in_fd = open(in_name, O_RDONLY)) < 0) {
		fprintf(stderr, "File \"%s\" could not be opened: %s\n",
				in_name, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if ((out_fd = open(out_name, O_CREAT | O_TRUNC | O_RDWR,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) < 0) {
		fprintf(stderr, "File \"%s\" could not be opened: %s\n",
				out_name, strerror(errno));
		exit(EXIT_FAILURE);
	}

	/* write header */
	write(out_fd, &nchars, sizeof(int));
	for (i = 0; i < MAX_HEAP_SIZE; i++)
		if (ftable[i] > 0) {
			ch = (unsigned char) i;
			write(out_fd, &ch, 1);
			write(out_fd, &ftable[i], sizeof(int));
		}

	/* encode */
	b = 0;
	bi = 7;
	j = 0;
	tchars = 0;
	while ((nbytes = read(in_fd, inbuf, BUF_SIZE)) > 0)
		for (i = 0; i < nbytes; i++) {
			ci = htable[inbuf[i]].length - 1;
			tchars++;
			while (ci >= 0) {
				if (htable[inbuf[i]].code & (1 << ci))
					b |= 1 << bi;
				ci--;
				bi--;
				if (bi == -1) {
					outbuf[j++] = b;
					b = 0;
					bi = 7;
					if (j == BUF_SIZE) {
						write(out_fd, &outbuf, BUF_SIZE);
						j = 0;
					}
				}
			}
		}

	if (bi >= 0)
		outbuf[j++] = b;
	if (j > 0)
		write(out_fd, &outbuf, j);

	/* close files */
	close(in_fd);
	close(out_fd);

	/* release data structures */
	free(out_name);
	huffman_delete_tree(htree);

	return EXIT_SUCCESS;
}

/* --- library -------------------------------------------------------------- */

/**
 * Deallocates the Huffman tree specified by <code>root</code>.
 *
 * @param[in]  root  the root of the Huffman tree
 */
void huffman_delete_tree(HeapNode *root)
{
	if (root->left != NULL)
		huffman_delete_tree(root->left);
	if (root->right != NULL)
		huffman_delete_tree(root->right);
	free(root);
}

/**
 * Reads the table of character frequences from the file descriptor.  This
 * function assumes the caller allocates space for the table.
 *
 * @param[in]   fd    the file descriptor from which to read
 * @param[out]  freq  the table of character frequencies, implicitly indexed by
 *                    ASCII codepoint
 */
void charfreq(int fd, int freq[])
{
	unsigned int i;
	unsigned char buf[BUF_SIZE];
	size_t nbytes;

	memset(freq, 0, MAX_HEAP_SIZE * sizeof(int));
	memset(buf, 0, BUF_SIZE);
	while ((nbytes = read(fd, buf, BUF_SIZE)) > 0)
		for (i = 0; i < nbytes; i++)
			freq[buf[i]]++;
}

/**
 * Displays the character frequencies in the <code>table</code>.
 *
 * @param[in]  table  the table of character frequencies
 */
void showfreq(int table[])
{
	int i;

	for (i = 0; i < MAX_HEAP_SIZE; i++)
		fprintf(STREAM, "%3d: %8d\n", i, table[i]);
}
