#ifndef __SEMANTIC_ACTIONS_H__
#define __SEMANTIC_ACTIONS_H__

#include <string.h>

#include "hashtable.h"
#include "stack.h"
#include "lexer.h"
#include "xstring.h"
#include "error.h"

#define SYM_TABLE_SIZE 100

// typedef struct _symbol_table {
//   Hashtable table;
//   _symbol_table *father_symbol_table;
// } SymbolTable;
// 
// SymbolTable* sym_table_new() {
//   
// }

typedef void Descriptor;

typedef struct _variable_stack_item {
  char *label;
  char *value;
} VariableStackItem;

typedef enum e_label_type {
  L_VARIABLE,
  L_CONSTANT,
  L_TEMP
} label_t;

typedef struct _variable_descriptor {
  class_t type;
  char* default_value;
} VariableDescriptor;

typedef struct _sym_table_entry {
  char* id_name;
  char* label;
  Descriptor* descriptor;
} SymTableEntry;

void semantic_initialize();

void sym_table_insert(char* id_name, char* label, Descriptor* descriptor);

SymTableEntry* sym_table_get(char* id_name);

int is_identifier_declared(char* id_name);

char* generate_label(int counter, label_t type);

/* CODE GENERATION */

void generate_code();
void mult_div_semantic_action(char *operator);
void add_sub_semantic_action(char *operator);
void rpar_semantic_action();  
void end_expr_semantic_action();
void generate_assignment_code(char *lvalue);
void generate_end_program_code();
void generate_data_definition_code();

#endif