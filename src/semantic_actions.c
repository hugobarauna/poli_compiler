#include "semantic_actions.h"

int variables_counter = 0;
Hashtable *sym_table;
Stack operators_stack;
Stack operands_stack;

void sym_table_initialize() {
  sym_table = hashtable_new(SYM_TABLE_SIZE);
  stack_clean(&operators_stack);
  stack_clean(&operands_stack);
}

void sym_table_insert(char* id_name, char* id_type, IdentifierDescriptor descriptor) {
  SymTableEntry* entry = new_sym_table_entry(id_name, id_type, descriptor);
  hashtable_insert(sym_table, id_name, entry);
}

SymTableEntry* sym_table_get(char* id_name) {
  return hashtable_get(sym_table, id_name);
}

static SymTableEntry* new_sym_table_entry(char* id_name, char* id_type, 
                                          IdentifierDescriptor descriptor) {
  SymTableEntry* entry = (SymTableEntry*) malloc(sizeof(SymTableEntry));
  entry->id_name = id_name;
  entry->id_type = id_type;
  entry->descriptor = descriptor;
  return entry;                                           
}