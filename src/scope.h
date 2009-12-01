#ifndef __H_SCOPE__
#define __H_SCOPE__

#include "lexer.h"
#include "hashtable.h"

#define SYM_TABLE_SIZE 100


typedef struct scope {
  Hashtable *table;
  struct scope *next;
} Scope;

typedef void Descriptor;

typedef struct _variable_descriptor {
  class_t type;
  char* name;
  char* default_value;
} VariableDescriptor;

typedef struct _routine_descriptor {
  class_t return_type;
  char *name;
  int num_params;
  VariableDescriptor params[10];
} RoutineDescriptor;

typedef struct _sym_table_entry {
  char* id_name;
  char* label;
  Descriptor* descriptor;
} SymTableEntry;

int scope_numbers();
void scope_reset();
void scope_new();
void scope_remove();
void scope_sym_table_insert(char* id_name, char* label, Descriptor* descriptor);
SymTableEntry* scope_sym_table_get(char* id_name);
SymTableEntry *scope_search_identifier(char *id_name);
int is_identifier_declared(char* id_name);

#endif