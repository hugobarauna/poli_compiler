#include "hashtable.h"
#include <string.h>

static Entry* new_entry(KEY key, VALUE value);

static int str_hash(char *str) {
    int result = 0;
    char *ptr = str;
    for (; *ptr; ptr++)
        result = 17 * result + *ptr;
    return result;
}

Hashtable* hashtable_new(unsigned int size) {
  Hashtable *new_ht = (Hashtable *)malloc(sizeof(Hashtable));
  new_ht->size = size;

  // allocating table space
  size_t entries_list_size = new_ht->size * sizeof(Entry *);
  new_ht->entries = (Entry **) malloc(entries_list_size);

  // fails them free hashtable
  if (new_ht->entries == NULL) { free(new_ht); return NULL; }

  // set table entry pointers to NULL
  memset(new_ht->entries, 0, entries_list_size);

  new_ht->hashing_function = str_hash;

  return new_ht;
}

void hashtable_delete(Hashtable *table) {
  int i;
  for (i = 0; i < table->size; i++) 
    free(table->entries[i]);

  free(table);
}

void hashtable_insert(Hashtable *table, KEY key, VALUE value) {
  int internal_key;
  internal_key = table->hashing_function(key) % table->size;

  /* No colision */
  if (table->entries[internal_key] == NULL) {
    Entry* entry = new_entry(key, value);
    table->entries[internal_key] = entry;
  }
  else {
    Entry *entry = new_entry(key, value);

    Entry *previous_entry = table->entries[internal_key];
    previous_entry->next = entry;
  }
}

VALUE hashtable_get(Hashtable *table, KEY key) {
  Entry *entry;
  int internal_key;
  internal_key = table->hashing_function(key) % table->size;

  entry = table->entries[internal_key];
  if (entry == NULL) return NULL;

  while ( entry != NULL && strcmp(entry->key, key) ) {
    entry = entry->next;
  }

  return entry == NULL ? NULL : entry->value;
}


static Entry* new_entry(KEY key, VALUE value) {
  Entry* entry = (Entry *) malloc(sizeof(Entry));
  entry->key = key;
  entry->value = value;
  entry->next = NULL;
  return entry;
}
