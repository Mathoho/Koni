#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tut5.h"

/* --- function prototypes -------------------------------------------------- */

Node *mknode(const char *name);
void  enumerate(const Node *root);
int   equal(const Node *r1, const Node *r2);
void  insnode(Node **root, Node *n);
void  delnode_c(Node **root, const char *name);

/* --- main routine --------------------------------------------------------- */

int main(int argc, char *argv[])
{
	char name[NAME_LEN+1], target[NAME_LEN+1];
	Node *root_asm, *root_c;

	/* check command-line arguments */
	if (argc < 1)
		goto usage;

	/* read data */
	root_asm = root_c = NULL;

	if (fgets(target, NAME_LEN + 1, stdin)) 
		target[strlen(target)-1] = '\0';
	else {
		fprintf(stderr, "Input stream does not have a target to delete.\n");
		goto usage;
	}

	while (fgets(name, NAME_LEN + 1, stdin)) {
		name[strlen(name)-1] = '\0';
		insnode(&root_asm, mknode(name));
		insnode(&root_c, mknode(name));
	}

	/* test */
	delnode_c(&root_c, target);
	delnode(&root_asm, target);

	printf("delnode for name '%s': ", target);
	if (equal(root_c, root_asm))
		printf("\033[32mcorrect\033[m\n");
	else {
		printf("\033[31mincorrect\033[m; BST");
		if (root_asm) {
			printf(" contains\n");
			enumerate(root_asm);
		} else
			printf(" is NULL\n");
		if (root_c) {
			printf("and should contain\n");
			enumerate(root_c);
		} else
			printf("and should be NULL\n");
	}

	return EXIT_SUCCESS;

usage:
	fprintf(stderr, "usage: %s\n", argv[0]);
	return EXIT_FAILURE;
}

/* --- library -------------------------------------------------------------- */

/**
 * Makes a new node for a binary search tree (BST), with the specified
 * <code>name</code> as element.  Should memory allocation fail, this function
 * terminates the program.
 *
 * @param[in]  name  the element of the node
 * @return     the new BST node, with the specified name as element
 */
Node *mknode(const char *name)
{
	Node *n;
	if ((n = malloc(sizeof(Node))) == NULL) {
		fprintf(stderr, "Node memory allocation failed.\n");
		exit(EXIT_FAILURE);
	}

	strcpy(n->name, name);
	n->left = n->right = NULL;
	return n;
}

/**
 * Enumerates the binary search tree (BST) rooted at <code>root</code> to the
 * standard output stream.  This is to say, displays the elements per in-order
 * left-to-right recursive tree traversal.
 *
 * @param[in]  root  the root of the BST
 */
void enumerate(const Node *root)
{
	if (root) {
		enumerate(root->left);
		printf("%s\n", root->name);
		enumerate(root->right);
	}
}

/**
 * Returns whether or not two binary search trees (BSTs) are equal.  Two BSTs
 * are equal it both root nodes are NULL, or if both root nodes contain the same
 * name, both left subtrees are equal, and both right subtrees are equal.
 *
 * @param[in]  r1  the root of the first BST to test
 * @param[in]  r2  the root of the second BST to test
 * @return     1 if both subtrees are equal; otherwise, 0
 */
int equal(const Node *r1, const Node *r2)
{
	if (r1 == NULL && r2 == NULL)
		return 1;
	else
		return (r1 != NULL && r2 != NULL
				&& strcmp(r1->name, r2->name) == 0
				&& equal(r1->left, r2->left)
				&& equal(r1->right, r2->right));
}

/**
 * Inserts the specified node into the specified binary search tree (BST).
 * Nodes with duplicate names are not inserted.
 *
 * @param[in,out]  root  the root of the BST
 * @param[in]      n     the node to insert into the BST
 */
void insnode(Node **root, Node *n)
{
	if (*root == NULL)
		*root = n;
	else {
		int cmp = strcmp(n->name, (*root)->name);
		if (cmp > 0)
			insnode(&(*root)->right, n);
		else if (cmp < 0)
			insnode(&(*root)->left, n);
	}
}

/**
 * Removes the maximum node from a binary search tree (BST).
 *
 * @param[in,out]  root  the root of the BST
 * @param[out]     max   the maximum node that was removed
 */
void rmmax_c(Node **root, Node **max)
{
	if ((*root)->right)
		rmmax_c(&(*root)->right, max);
	else {
		*max = *root;
		*root = (*max)->left;
	}
}

/**
 * Locates a node whose name field matches the name specified as a parameter,
 * and if such a node exists, deletes it, and reorganises the binary search tree
 * if necessary). The memory occupied by the removed node is also released using
 * the free function.
 *
 * @param[in,out]   root  reference parameter to the root node of a binary tree
 * @param[in]       name  name field of the node that must be deleted
 */
void delnode_c(Node **root, const char *name)
{
	if (*root) {
		int cmp = strcmp(name, (*root)->name);
		if (cmp < 0)
			delnode_c(&(*root)->left, name);
		else if (cmp > 0)
			delnode_c(&(*root)->right, name);
		else {
			Node *n, *tmp;
			tmp = *root;
			if ((*root)->left == NULL)
				*root = (*root)->right;
			else if ((*root)->right == NULL)
				*root = (*root)->left;
			else {
				rmmax_c(&(*root)->left, &n);
				n->left = (*root)->left;
				n->right = (*root)->right;
				*root = n;
			}
			free(tmp);
		}
	}
}
