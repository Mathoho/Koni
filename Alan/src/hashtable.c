/**
 * @file    hashtable.c
 * @brief   A generic hash table.
 * @author  W.H.K. Bester (whkbester@cs.sun.ac.za)
 * @date    2022-08-03
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"

#define INITIAL_DELTA_INDEX  4
#define PRINT_BUFFER_SIZE 1024

/** an entry in the hash table */
typedef struct htentry HTentry;
struct htentry {
	void    *key;       /*<< the key                      */
	void    *value;     /*<< the value                    */
	HTentry *next_ptr;  /*<< the next entry in the bucket */
};

/** a hash table container */
struct hashtab {
	/** a pointer to the underlying table                              */
	HTentry **table;
	/** the current size of the underlying table                       */
	unsigned int size;
	/** the current number of entries                                  */
	unsigned int num_entries;
	/** the maximum load factor before the underlying table is resized */
	float max_loadfactor;
	/** the index into the delta array                                 */
	unsigned short idx;
	/** a pointer to the hash function                                 */
	unsigned int (*hash)(void *, unsigned int);
	/** a pointer to the comparison function                           */
	int (*cmp)(void *, void *);
};

/* --- function prototypes -------------------------------------------------- */

/* TODO: For the following functions, refer to the TODO note at the end of the
 * file.
 */

static int getsize(HashTab *ht);
static HTentry **talloc(int tsize);
static void rehash(HashTab *ht);

/* TODO: For this implementation, we want to ensure we *always* have a hash
 * table that is of prime size.  To that end, the next array stores the
 * different between a power of two and the largest prime less than that
 * particular power-of-two.  When you rehash, compute the new prime size using
 * the following array.
 */

/** the array of differences between a power-of-two and the largest prime less
 * than that power-of-two.                                                    */
unsigned short delta[] = { 0, 0, 1, 1, 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15,
1, 5, 1, 3, 9, 3, 15, 3, 39, 5, 39, 57, 3, 35, 1 };

#define MAX_IDX (sizeof(delta) / sizeof(unsigned short))

/* --- hash table interface ------------------------------------------------- */

HashTab *ht_init(float loadfactor,
				 unsigned int (*hash)(void *, unsigned int),
				 int (*cmp)(void *, void *))
{
	HashTab *ht;
	unsigned int i;
	/* TODO:
	 * - Initialise a hash table structure by calling an allocation function
	 *   twice://WHAT THEY MEAN IS WE MUST USE MALLOC,REALLOC etc
	 *   (1) once to allocate space for a HashTab variable, and
	 *   (2) once to allocate space for the table field of this new HashTab
	 *       variable.
	 * - If any allocation fails, free anything that has already been allocated
	 *   successfully, and return NULL.
	 * - Also set up the other fields of the newly-allocated HashTab structure
	 *   appropriately.
	 */
	 //HASH VARIABLE
    ht = malloc(sizeof(HashTab));
	if (ht == NULL) {
		return NULL;
	}
	int power = 1;
	int j;
	for (j = 0; j < INITIAL_DELTA_INDEX; j++) {
		power=power*2;
	}
	//table
	HTentry **tab;
	ht->size = power - delta[INITIAL_DELTA_INDEX];
	tab = malloc(ht->size * sizeof(HTentry));
	ht->table = tab;
	if (ht->table == NULL) {
		free(ht);
		return NULL;
	}
	//set other fields
	ht->num_entries = 0;
	ht->max_loadfactor = loadfactor;
	ht->hash = hash;
	ht->cmp = cmp;
	ht->idx = INITIAL_DELTA_INDEX;
	for (i = 0; i < ht->size; i++) {
		ht->table[i] = NULL;
	}
	return ht;
}

int ht_insert(HashTab *ht, void *key, void *value)
{
	int k;
	HTentry *p;//IS THE new table i am making

	/* TODO: Insert a new key--value pair, rehashing if necessary.  The best way
	 * to go about rehashing is to put the necessary elements into a static
	 * function called rehash.  Remember to free space (the "old" table) you not
	 * use any longer.  Also, if something goes wrong, use the #define'd
	 * constants in hashtable.h for return values; remember, unless it runs out
	 * of memory, no operation on a hash table may terminate the program.
	 */
	if (ht->size < 0) {
		return HASH_TABLE_NO_SPACE_FOR_NODE;
	}

	k = ht->hash(key, ht->size);
	p = malloc(sizeof(HTentry));
	p->key = key;
	p->value = value;
	if (ht->table[k]) {
		HTentry *nxt;
		HTentry *prv = NULL;
		nxt = ht->table[k];
		while (nxt != NULL) {
			if (ht->cmp(nxt->key, key) == 0) {
				free(p);
				return HASH_TABLE_KEY_VALUE_PAIR_EXISTS;
			}
			prv = nxt;
			nxt = nxt->next_ptr;
		}
		ht->num_entries+=1;
		prv->next_ptr = p;
		p->next_ptr = NULL;
	} else if (ht->table[k] == NULL) {
		ht->table[k] = p;
		ht->table[k]->next_ptr = NULL;
		ht->num_entries+=1;
	}
	float loadfactor = (float) ht->num_entries/ht->size;
	if (loadfactor > ht->max_loadfactor) {
		rehash(ht);
	}
	return EXIT_SUCCESS;
}

Boolean ht_search(HashTab *ht, void *key, void **value)
{
	int k;
	HTentry *p;
	/* TODO: Nothing!  This function is complete, and should explain by example
	 * how the hash table looks and must be accessed.
	 */
	p = NULL;
	k = ht->hash(key, ht->size);
	for (p = ht->table[k]; p; p = p->next_ptr) {
		if (ht->cmp(key, p->key) == 0) {
			*value = p->value;
			break;
		}
	}
	return (p ? TRUE : FALSE);
}

int ht_free(HashTab *ht, void (*freekey)(void *k), void (*freeval)(void *v))
{
	unsigned int i;
	HTentry *p, *q;
	/* free the nodes in the buckets */
	/* TODO */
	for (i = 0; i < ht->size; i++) {
		p = ht->table[i];
		while (p) {
			q = p->next_ptr;
			freeval(p->value);
			freekey(p->key);
			free(p);
			p = q;
		}
	}
	/* free the table and container */
	/* TODO */
	free(ht->table);
	if (ht->table) {
		ht->table = NULL;
		free(ht);
		return EXIT_FAILURE;
	}
	free(ht);
	if (ht) {
		ht = NULL;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void ht_print(HashTab *ht, void (*keyval2str)(void *k, void *v, char *b))
{
	unsigned int i;
	HTentry *p;
	char buffer[PRINT_BUFFER_SIZE];

	/* TODO: This function is complete and useful for testing, but you have to
	 * write your own keyval2str function if you want to use it.
	 */
	for (i = 0; i < ht->size; i++) {
		printf("bucket[%2i]", i);
		for (p = ht->table[i]; p != NULL; p = p->next_ptr) {
			keyval2str(p->key, p->value, buffer);
			printf(" --> %s", buffer);
		}
		printf(" --> NULL\n");
	}
}

/* --- utility functions ---------------------------------------------------- */

/* TODO: I suggest completing the following helper functions for use in the
 * global functions ("exported" as part of this unit's public API) given above.
 * You may, however, elect not to use them, and then go about it in your own way
 * entirely.  The ball is in your court, so to speak, but remember: I have
 * suggested using these functions for a reason -- they should make your life
 * easier.
 */

static int getsize(HashTab *ht)
{
	/* TODO: Compute the next prime size of the hash table. */
	unsigned int kidx = ht->idx + 1;
	int power = 1;
	unsigned int j;
    for (j = 0; j < kidx; j++) {
      power = power*2;
	}
	if (kidx < MAX_IDX) {
		int new_size = power - delta[kidx];
		return new_size;
	}
	return HASH_TABLE_NO_SPACE_FOR_NODE;
}
static HTentry **talloc(int tsize)
{
	/* TODO: Allocate space for one hash table entry; this is useful if your
	 * source code lines become rather long, and you have to wrap. */
	 HTentry **q;
	 q = malloc(tsize*sizeof(HTentry));
	 return q;
}

static void rehash(HashTab *ht)
{
	/* TODO: Rehash the hash table by
	 * (1) allocating a new table that uses as size the next prime in the
	 *     "almost-double" array,
	 * (2) moving the entries in the existing table to appropriate positions in
	 *     the new table, and
	 * (3) freeing the old table.
	 */
	 //new table size and allocation
	unsigned int oldtab_size = ht->size;
	ht->size = oldtab_size;
	unsigned int newtab_size = getsize(ht);
	ht->size = newtab_size;
	if (newtab_size > 0) {
		unsigned int ij, k;
		HTentry *nx;
		HashTab *old_tab;
		HTentry **new_tab;
		old_tab = (HashTab*) malloc(sizeof(HashTab));
		old_tab->table = malloc(oldtab_size * sizeof(HTentry));
		for (k = 0; k < oldtab_size; k++) {
			old_tab->table[k] = ht->table[k];
		}
		new_tab = talloc(ht->size);
		unsigned int i, new_indx;
		free(ht->table);
		ht->table = new_tab;
		ht->num_entries = 0;
		for (ij = 0; ij < ht->size; ij++) {
			ht->table[ij] = NULL;
		}
		for (i = 0; i < oldtab_size; i++) {
			for (nx = old_tab->table[i]; nx; nx = nx->next_ptr) {
				new_indx = ht->hash(nx->key, ht->size);
				if (ht->table[new_indx]) {
					HTentry *nxt;
					nxt = malloc(sizeof(HTentry));
					HTentry *prv = NULL;
					prv = malloc(sizeof(HTentry));
					nxt = ht->table[new_indx];
					while (nxt != NULL) {
						prv = nxt;
						nxt = nxt->next_ptr;
					}
					ht->num_entries+=1;
					prv->next_ptr = nx;
					prv->next_ptr->next_ptr = NULL;
				} else {
					ht->table[new_indx] = nx;
					ht->table[new_indx]->next_ptr = NULL;
					ht->num_entries+=1;
				}
			}
		}
	ht->idx = ht->idx + 1;
	free(old_tab->table);
	old_tab->table = NULL;
	free(old_tab);
	old_tab = NULL;
	 }
}
