#include "scope.h"
#include "error.h"

static Scope *current_scope = NULL;
static int _scope_numbers = 0;

static SymTableEntry* new_sym_table_entry(char* id_name, char* label, 
          Descriptor* descriptor) {
  SymTableEntry* entry = (SymTableEntry*) malloc(sizeof(SymTableEntry));
  entry->id_name = id_name;
  entry->label = label;
  entry->descriptor = descriptor;
  return entry;                                           
}

int scope_numbers() {
  return _scope_numbers;
}

void scope_reset() {
  current_scope = NULL;
  _scope_numbers = 0;
}

void scope_new() {
  Scope *scope;
  scope = (Scope *) malloc(sizeof(Scope));
  if (scope == NULL)
    fatal_error("Couldnt create a new scope");
  
  scope->table = hashtable_new(1024);
  if (current_scope == NULL) {
    current_scope = scope;
  } else {
    scope->next = current_scope;
    current_scope = scope;
  }
  printf("! NEW scope declared\n");
  _scope_numbers++;
}

void scope_remove() {
  if (current_scope->next == NULL)
    fatal_error("Can't remove global scope");
  
  current_scope = current_scope->next;
  _scope_numbers--;
}

void scope_sym_table_insert(char *id_name, char *label, Descriptor *descriptor) {
  SymTableEntry* entry = new_sym_table_entry(id_name, label, descriptor);
  printf("[[[[[[ %s, %s\n", id_name, label);
  hashtable_insert(current_scope->table, id_name, entry);
}

SymTableEntry* scope_sym_table_get(char *id_name) {
  return scope_search_identifier(id_name);
}

/*
void scope_sym_table_remove(char *id_name) {
  return hashtable_remove(current_scope->table, id_name);
}
*/

SymTableEntry *scope_search_identifier(char *id_name) {
  SymTableEntry *entry = NULL;
  Scope *scope = current_scope;
  
  printf("CURRENT SCOPE: %d\n", scope_numbers());
  printf("id_name: %s\n", id_name);
  /* first search on current scope */
  entry = hashtable_get(scope->table, id_name);
  
  if (entry != NULL)
    return entry;
  
  /* try search */
  scope = scope->next;
  while (scope != NULL) {
    entry = hashtable_get(scope->table, id_name);
    scope = scope->next;
  }
  printf("?entry: %s, %s\n", entry->label, entry->id_name);
  return entry;
}

int is_identifier_declared(char* id_name) {
  return scope_sym_table_get(id_name) != NULL;
}

int is_identifier_locally_declared(char *id_name) {
  return hashtable_get(current_scope->table, id_name) != NULL;
}
