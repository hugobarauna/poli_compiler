/*#define HASHTABLE_SIZE 100*/
#include <malloc.h>
#define KEY char
#define VALUE char


typedef struct _entry {
    KEY k;
    VALUE v;
    struct _entry *next;
} Entry;

typedef struct _hashtable {
    Entry* *entries;
    unsigned int size;
} Hashtable;

// create a new instance of hashtable
Hashtable *new_hashtable(unsigned int size);

// insert a <key, value> pair to hashtable
void hashtable_insert(Hashtable *ht, KEY *k, VALUE *v);

// retrieve the value from key
VALUE hashtable_get(Hashtable *ht, KEY *k);

/*TODO*/
/*// free hashtable memory*/
/*void free_hashtable(Hashtable *ht);*/
