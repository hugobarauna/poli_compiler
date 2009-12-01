/*#define HASHTABLE_SIZE 100*/
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY   char*
#define VALUE void*

typedef struct _entry {
    KEY key;
    VALUE value;
    struct _entry *next;
} Entry;

typedef struct _hashtable {
    Entry* *entries;
    unsigned int size;
    int (*hashing_function) (KEY key);
} Hashtable;

/* Create a new instance of hashtable */
Hashtable *hashtable_new(unsigned int size);

/* Free hashtable's alocated memory */
void hashtable_delete(Hashtable *table);

/* Insert a <key, value> pair into the hashtable */
void hashtable_insert(Hashtable *table, KEY key, VALUE value);
void hashtable_remove(Hashtable *table, KEY key);

VALUE hashtable_get(Hashtable *table, KEY key);


#endif