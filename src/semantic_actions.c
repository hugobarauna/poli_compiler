#include "semantic_actions.h"

int variables_counter = -1;
int constants_counter = -1;
Hashtable *sym_table;
Stack operators_stack, operands_stack, constants_stack, variables_stack;

static SymTableEntry* new_sym_table_entry(char* id_name, char* label, Descriptor* descriptor);
static void clean_stacks();

void sym_table_initialize() {
  sym_table = hashtable_new(SYM_TABLE_SIZE);
  clean_stacks();
  /* Maybe here, I already can open the output .asm file and write the first headers,
     somethin like this
           @ /0
  */
}

void sym_table_insert(char* id_name, char* label, Descriptor* descriptor) {
  SymTableEntry* entry = new_sym_table_entry(id_name, label, descriptor);
  hashtable_insert(sym_table, id_name, entry);
}

SymTableEntry* sym_table_get(char* id_name) {
  return hashtable_get(sym_table, id_name);
}

int is_identifier_declared(char* id_name) {
  if (sym_table_get(id_name) != NULL)
    return 1;
  else
    return 0;
}

char* generate_label(int counter, label_t type) {
  char* label;
  label = (char* ) malloc(5);

  switch (type) {
    case L_VARIABLE:
      strcpy(label, "V");
      break;
    case L_CONSTANT:
      strcpy(label, "C");
    default:
      fatal_error("Error: invalid label type.");
  }

  strcat(label, "0");
  return label;
}

static SymTableEntry* new_sym_table_entry(char* id_name, char* label, Descriptor* descriptor) {
  SymTableEntry* entry = (SymTableEntry*) malloc(sizeof(SymTableEntry));
  entry->id_name = id_name;
  entry->label = label;
  entry->descriptor = descriptor;
  return entry;                                           
}

static void clean_stacks() {
  stack_clean(&operators_stack);
  stack_clean(&operands_stack);
  stack_clean(&constants_stack);
  stack_clean(&variables_stack);
}