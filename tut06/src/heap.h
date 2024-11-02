#ifndef HEAP_H
#define HEAP_H

#define MAX_HEAP_SIZE 256

/* set stream for debugging output */
#ifdef DEBUG
	#define STREAM stderr
#else
	#define STREAM stdout
#endif

typedef struct heap_node HeapNode;
struct heap_node {
	int            frequency;  /*<< the character frequency              */
	unsigned char  c;          /*<< the character                        */
	HeapNode      *left;       /*<< the left node (in the Huffman tree)  */
	HeapNode      *right;      /*<< the right node (in the Huffman tree) */
};

typedef struct heap {
	int n;                      /*<< the number of elements in the heap      */
	HeapNode a[MAX_HEAP_SIZE];  /*<< the array for storing the heap elements */
} Heap;

/**
 * Initialises the heap to all zero bits.
 *
 * @param[in,out]  H  the heap
 */
void heap_initialize(Heap *H);

/**
 * Removes the minimum node in the heap, and then reorganises the heap so that
 * the new minimum node is in the correct position.
 *
 * @param[in,out]  H     the heap
 * @param[out]     node  the removed minimum node
 */
void heap_remove(Heap *H, HeapNode *node);

/**
 * Inserts a new node into the heap.
 *
 * @param[in,out]  H     the heap
 * @param[in]      node  the node to insert into the heap
 */
void heap_insert(Heap *H, HeapNode *node);

/**
 * Displays the heap on the standard output stream.
 *
 * @param[in]  H  the heap
 */
void show_heap(Heap *H);

#endif
