#include "semantic_actions.h"

int variables_counter = -1;
int constants_counter = -1;
int temps_counter = -1;
Hashtable *sym_table;
Stack operators_stack, operands_stack, constants_stack, variables_stack;

static SymTableEntry* new_sym_table_entry(char* id_name, char* label, Descriptor* descriptor);
static void clean_stacks();

static FILE *fp = NULL;

void semantic_initialize() {
  sym_table = hashtable_new(SYM_TABLE_SIZE);
  clean_stacks();
  fp = fopen("out.asm", "w");
  fputs("\t@ /0\n", fp);
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
  char* label = (char* ) malloc(10);
  if (label == NULL)
    fatal_error("failed: alloc label");

  switch (type) {
    case L_VARIABLE:
      strcpy(label, "V");
      break;
    case L_CONSTANT:
      strcpy(label, "C");
      break;
    case L_TEMP:
      strcpy(label, "TEMP");
      break;
    default:
      fatal_error("Error: invalid label type.");
  }

  strcat(label, itoa(counter));
  return label;
}

void generate_code() {
  // GERA CODIGO
  VariableStackItem *lvalue, *rvalue, *operator;
  VariableStackItem *temp;
  
  operator = stack_pop(&operators_stack);
  rvalue = stack_pop(&operands_stack);
  lvalue = stack_pop(&operands_stack);
  
  fputs("\tLD\t", fp);
  fputs(lvalue->label, fp);
  fputs("\n\t", fp);
  fputs(operator->label, fp);
  fputs("\t", fp);
  fputs(rvalue->label, fp);
  fputs("\n", fp);

  temps_counter++;
  temp = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  temp->label = generate_label(temps_counter, L_TEMP);
  temp->value = "0";

  fputs("\tMM\t", fp);
  fputs(temp->label, fp);
  fputs("\n", fp);

  /* register temp variable to be generated on data segment */
  stack_push(temp, &variables_stack);
  stack_push(temp, &operands_stack);
}

void mult_div_semantic_action(char *operator) {
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item != NULL
      && (strcmp(item->label, "*") == 0 
      || strcmp(item->label, "/") == 0)) {
      generate_code();
      mult_div_semantic_action(operator);
  }
  else {
    item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
    item->label = operator; /* *, / */
    item->value = NULL;
  
    stack_push(item, &operators_stack);
  }
}

void add_sub_semantic_action(char *operator) {
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item != NULL
      && (strcmp(item->label, "*") == 0 
      || strcmp(item->label, "/") == 0
      || strcmp(item->label, "-") == 0
      || strcmp(item->label, "+") == 0)) {
    generate_code();
    add_sub_semantic_action(operator);
  }
  else {
    item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
    item->label = operator; /* +, - */
    item->value = NULL;
  
    stack_push(item, &operators_stack);
  }
}

void rpar_semantic_action() {
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item != NULL && strcmp(item->label, "(") != 0) {
    generate_code();
    rpar_semantic_action();
  }
  else {
    stack_pop(&operators_stack);
  }
}

void generate_assignment_code(char *lvalue) {
  VariableStackItem *item = NULL;
  
  if (lvalue == NULL)
    fatal_error("Error: lvalue should not be NULL");
    
  /* FINAL CONDITION: OPERANDS CONTAINS THE EXPR RESULT */
  item = stack_pop(&operands_stack);
  fputs("\tLD\t", fp);
  fputs(item->label, fp);
  fputs("\n", fp);
    
  fputs("\tMM\t", fp);
  fputs(lvalue, fp);
  fputs("\n", fp);
}

void end_expr_semantic_action() {
  VariableStackItem *item = stack_lookup(&operators_stack);
  while (item != NULL && strcmp(item->label, "(") != 0) {
    generate_code();
    item = stack_lookup(&operators_stack);
  }
}

void generate_end_program_code() {
  fputs("\tHM /0\n", fp);
}

void generate_data_definition_code() {
  fputs("\t@ /200\n", fp);
  while (!stack_empty(&variables_stack)) {
    VariableStackItem *item = (VariableStackItem *)stack_pop(&variables_stack);
    fputs(item->label, fp);
    fputs("\tK\t=", fp);
    fputs(item->value, fp);
    fputs("\n", fp);
  }
  
  while (!stack_empty(&constants_stack)) {
    VariableStackItem *item = (VariableStackItem *)stack_pop(&constants_stack);
    fputs(item->label, fp);
    fputs("\tK\t=", fp);
    fputs(item->value, fp);
    fputs("\n", fp);
  }
  fclose(fp);
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