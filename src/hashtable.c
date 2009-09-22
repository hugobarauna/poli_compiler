#include "hashtable.h"

Hashtable *new_hashtable(unsigned int size) {
  Hashtable *new_ht = (Hashtable *)malloc(sizeof(Hashtable));
  new_ht->size = size;

  // allocating table space
  size_t entries_list_size = new_ht->size * sizeof(Entry *);
  new_ht->entries = (Entry **) malloc(entries_list_size);

  // fails them free hashtable
  if (new_ht->entries == NULL) { free(new_ht); return NULL; }

  // set table entry pointers to NULL
  memset(new_ht->entries, NULL, entries_list_size);

  return new_ht;
}
