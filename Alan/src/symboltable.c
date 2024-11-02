/**
 * @file    symboltable.c
 * @brief   A symbol table for ALAN-2022.
 * @author  W. H. K. Bester (whkbester@cs.sun.ac.za)
 * @date    2022-08-03
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boolean.h"
#include "error.h"
#include "hashtable.h"
#include "symboltable.h"
#include "token.h"

/* --- global static variables ---------------------------------------------- */

static HashTab *table, *saved_table;
/* TODO: Nothing here, but note that the next variable keeps a running count of
 * the number of variables in the current symbol table.  It will be necessary
 * during code generation to compute the size of the local variable array of a
 * method frame in the Java virtual machine.
 */
static unsigned int curr_offset;

/* --- function prototypes -------------------------------------------------- */

static void valstr(void *key, void *p, char *str);
static void freeprop(void *p);
static unsigned int shift_hash(void *key, unsigned int size);
static int key_strcmp(void *val1, void *val2);

/* --- symbol table interface ----------------------------------------------- */

void init_symbol_table(void)
{
	saved_table = NULL;
	if ((table = ht_init(0.75f, shift_hash, key_strcmp)) == NULL) {
		eprintf("Symbol table could not be initialised");
	}
	curr_offset = 1;
}

Boolean open_subroutine(char *id, IDprop *prop)
{
	/* TODO:
	 * - Insert the subroutine name into the global symbol table; return TRUE or
	 *   FALSE, depending on whether or not the insertion succeeded.
	 * - Save the global symbol table to saved_table, initialise a new hash
	 *   table for the subroutine, and reset the current offset.
	 */
	int val = ht_insert(table,id,(void*) prop);
	if (val == 0) {
		saved_table = table;
		table = ht_init(0.75f, shift_hash, key_strcmp);
		curr_offset = 0;
		return TRUE;
	}
	return FALSE;
}

void close_subroutine(void)
{
	/* TODO: Release the subroutine table, and reactivate the global table. */

	//releasing the suroutine table
	if (table != NULL) {
		ht_free(table, free, freeprop);
	}
	//reactivating the global table
	table = saved_table;
	saved_table = NULL;
	curr_offset = 1;
}

Boolean insert_name(char *id, IDprop *prop)
{
	/* TODO: Insert the properties of the identifier into the hash table, and
	 * remember to increment the current offset pointer if the identifier is a
	 * variable.
	 *
	 * VERY IMPORTANT: Remember to read the documentation of this function in
	 * the header file.
	 */
	int tru = ht_insert(table,id,(void *) prop);
	if (tru < 0) {
		return FALSE;
	}
	//check if it is an variable
	if (IS_VARIABLE(prop->type)) {
		curr_offset++;
	}
	return TRUE;
}

Boolean find_name(char *id, IDprop **prop)
{
	Boolean found;

	/* TODO: Nothing, unless you want to.*/
	found = ht_search(table, id, (void **) prop);
	if (!found && saved_table) {
		found = ht_search(saved_table, id, (void **) prop);
		if (found && !IS_CALLABLE_TYPE((*prop)->type)) {
			found = FALSE;
		}
	}

	return found;
}

int get_variables_width(void)
{
	return curr_offset;
}

void release_symbol_table(void)
{
	/* TODO: Free the underlying structures of the symbol table. */
	if (table != NULL) {
		ht_free(table,free,freeprop);
	}
}

void print_symbol_table(void)
{
	ht_print(table, valstr);
}

/* --- utility functions ---------------------------------------------------- */

static void valstr(void *key, void *p, char *str)
{
	char *keystr = (char *) key;
	IDprop *idpp = (IDprop *) p;

	/* TODO: Nothing, but this should give you an idea of how to look at the
	 * contents of the symbol table.
	 */
	 idpp->offset = 0;
	sprintf(str, "%s.%d", keystr, idpp->offset);
	sprintf(str, "%s@%d[%s]", keystr, idpp->offset,
			get_valtype_string(idpp->type));
}

/* TODO: Here you should add your own utility functions, in particular, for
 * deallocation, hashing, and key comparison.  For hashing, you MUST NOT use the
 * simply strategy of summing the integer values of characters.  I suggest you
 * use some kind of cyclic bit shift hash.
 */

static void freeprop(void *p)
{
	IDprop *idprp = (IDprop *) p;
	free(idprp->params);
	free(idprp);

}

static unsigned int shift_hash(void *key, unsigned int size)
{
	char *str = (char *) key;
	int len = strlen(str);
	int hash = 0;
	int i;
	for (i = 0; i < len; i++) {
		hash = (hash << 5)|((unsigned int)hash >> 27);
		hash += str[i];
	}
	if (hash < 0) {
		hash *=-1;
	}
	unsigned int indx = hash % size;
	return indx;
}

static int key_strcmp(void *val1, void *val2)
{

	char *val11 = (char *) val1;
	char *val22 = (char *) val2;
	int cmp = strcmp(val11,val22);
return cmp;
}
