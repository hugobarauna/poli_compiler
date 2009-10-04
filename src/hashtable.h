/*#define HASHTABLE_SIZE 100*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY   char*
#define VALUE char*

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

VALUE hashtable_get(Hashtable *table, KEY key);

// insert a <key, value> pair to hashtable
//void hashtable_insert(Hashtable *ht, KEY *k, VALUE *v);

// retrieve the value from key
//VALUE hashtable_get(Hashtable *ht, KEY *k);

/*TODO*/
// free hashtable memory*/*/
/*void free_hashtable(Hashtable *ht);*/
