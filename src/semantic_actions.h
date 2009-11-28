#ifndef __SEMANTIC_ACTIONS_H__
#define __SEMANTIC_ACTIONS_H__

#include "hashtable.h"

#define SYM_TABLE_SIZE 100

// typedef struct _symbol_table {
//   Hashtable table;
//   _symbol_table *father_symbol_table;
// } SymbolTable;
// 
// SymbolTable* sym_table_new() {
//   
// }

typedef enum _identifier_descriptor {
  VARIABLE,
  ARRAY,
  FUCTION
} IdentifierDescriptor;

typedef struct _sym_table_entry {
  char* id_name;
  char* id_type;
  IdentifierDescriptor descriptor;
} SymTableEntry;

void sym_table_initialize();

void sym_table_insert(char* id_name, char* id_type, IdentifierDescriptor descriptor);

SymTableEntry* sym_table_get(char* id_name);

static SymTableEntry* new_sym_table_entry(char* id_name, char* id_type, 
  IdentifierDescriptor descriptor);

#endif