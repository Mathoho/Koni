#ifndef TUT5_H
#define TUT5_H

#define NAME_LEN 31

typedef struct {
	int  number;
	char name[NAME_LEN+1];
} Item;

typedef struct node_s Node;
struct node_s {
	char  name[NAME_LEN+1];
	Node *left;
	Node *right;
};

/**
 * Performs a binary insertion sort on the items pointed to by the parameter
 * list.
 *
 * @param[in,out]   list  pointer to an array of items
 * @param[in]       n     total number of elements in the array
 */
void binary_sort(Item *list, int n);

/**
 * Locates a node whose name field matches the name specified as a parameter,
 * and if such a node exists, deletes it, and reorganises the binary search tree
 * if necessary). The memory occupied by the removed node is also released using
 * the free function.
 *
 * @param[in,out]   root  reference parameter to the root node of a binary tree
 * @param[in]       name  name field of the node that must be deleted
 */
void delnode(Node **root, const char *name);

/**
 * Converts the specified integer value to a NULL terminated character string
 * that contains the binary representation of the value specified by n.
 *
 * @param[in]       n  any legal signed integer value
 * @param[out]      s  pointer to a character string which must be able to hold
 *                     33 characters
 */
void bin_to_string(int n, char *s);

#endif /* TUT5_H */
