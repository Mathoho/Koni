#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "heap.h"

typedef struct huffman_node {
	int code;    /*<< the code (bit) string                          */
	int length;  /*<< the length of the code string (number of bits) */
} HuffmanNode;

/**
 * Builds a Huffman tree <code>t</code> by consuming the minheap <code>h</code>.
 *
 * @param[in,out]  H  the minheap constructed to character frequencies
 * @param[out]     t  the root of the Huffman tree
 */
void huffman_build_tree(Heap *H, HeapNode **t);

/**
 * Initialises the Huffman table by filling its memory area with 1 bits.  This
 * function assumes that its called has already allocated space for
 * <code>t</code> by a call to <code>malloc</code>.
 *
 * @param  t  a pointer to the space allocated for the Huffman table
 */
void huffman_initialize_table(HuffmanNode *t);

/**
 * Builds a lookup table for the Huffman code by recursively traversing the
 * Huffman tree in-order.  This function assumes that space for the table has
 * been allocated by the caller.  The table is indexed by the 8-bit ASCII
 * character set.
 *
 * @param[in]   root  the node of the Huffman (sub)tree to visit next
 * @param[out]  t     the Huffman lookup table
 * @param[in]   code  the code string leading to the current root
 * @param[in]   len   the length of the current code string
 */
void huffman_build_table(HeapNode *root, HuffmanNode *t, int code, int len);

#endif
