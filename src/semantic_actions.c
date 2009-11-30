#include "semantic_actions.h"
#include "lexer.h"

class_t type_declared;
int variables_counter = -1;
int constants_counter = -1;
int ifs_counter = -1;
int elses_counter = -1;
int endifs_counter = -1;
int whiles_counter = -1;
int bools_counter = -1;
int temps_counter = -1;
Hashtable *sym_table;
Stack operators_stack, operands_stack, constants_stack, variables_stack;
Stack stmts_stack;
Token *bool_operator = NULL;
char *lvalue = NULL;

static SymTableEntry* new_sym_table_entry(char* id_name, char* label, Descriptor* descriptor);
static void clean_stacks();

static FILE *fp = NULL;

void semantic_initialize() {
  VariableStackItem *ktrue, *kfalse;
  
  sym_table = hashtable_new(SYM_TABLE_SIZE);
  clean_stacks();
  /* LOAD FIXED CONSTANTS */
  ktrue = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  ktrue->label = "TRUE";
  ktrue->value = "1";
  
  kfalse = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  kfalse->label = "FALSE";
  kfalse->value = "0";
  
  stack_push(ktrue, &constants_stack);
  stack_push(kfalse, &constants_stack);
  
  fp = fopen("out.asm", "w");
  fprintf(fp, "        @  /0\n");
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
      strcpy(label, "VAR_");
      break;
    case L_CONSTANT:
      strcpy(label, "K_");
      break;
    case L_TEMP:
      strcpy(label, "TEMP_");
      break;
    case L_IF:
      strcpy(label, "IF_");
      break;
    case L_ELSE:
      strcpy(label, "ELSE_");
      break;
    case L_ENDIF:
      strcpy(label, "ENDIF_");
      break;
    case L_WHILE:
      strcpy(label, "WHILE_");
      break;
    case L_ENDWHILE:
      strcpy(label, "ENDWHILE_");
      break;
    case L_TRUE:
      strcpy(label, "TRUE_");
      break;
    case L_FALSE:
      strcpy(label, "FALSE_");
      break;
    default:
      fatal_error("Error: invalid label type.");
  }

  strcat(label, itoa(counter));
  return label;
}

void generate_code() {
  // GERA CODIGO
  VariableStackItem *lvalue, *rvalue, *operator, *temp;
  
  operator = stack_pop(&operators_stack);
  rvalue = stack_pop(&operands_stack);
  lvalue = stack_pop(&operands_stack);
  
 /* fputs("\tLD\t", fp);
  fputs(lvalue->label, fp);
  fputs("\n\t", fp);
  fputs(operator->label, fp);
  fputs("\t", fp);
  fputs(rvalue  ->label, fp);
  fputs("\n", fp);
*/
  temps_counter++;
  temp = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  temp->label = generate_label(temps_counter, L_TEMP);
  temp->value = "0";
/*

  fputs("\tMM\t", fp);
  fputs(temp->label, fp);
  fputs("\n", fp);
*/
  fprintf(fp, "          LD  %s\n", lvalue->label);
  fprintf(fp, "          %-2s  %s\n", operator->label, rvalue->label);
  fprintf(fp, "          MM  %s\n", temp->label);
  
  printf("\t\t\t-----------------\n\t\t\t%s = %s %s %s\n\t\t\t-----------------\n", 
    temp->label, lvalue->label, operator->label, rvalue->label);

  /* register temp variable to be generated on data segment */
  stack_push(temp, &variables_stack);
  stack_push(temp, &operands_stack);
}

void mult_div_semantic_action(char *operator) {
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item == NULL) printf("\t\t\tTrace: mult-div no operator\n");
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
    printf("! %s pushed to stack\n", item->label);
    stack_push(item, &operators_stack);
  }
}

void add_sub_semantic_action(char *operator) {
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item == NULL) printf("\t\t\tTrace: add-sub no operator\n");
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
    printf("! %s pushed to stack\n", item->label);
    stack_push(item, &operators_stack);
  }
}

void rpar_semantic_action() {
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item != NULL && strcmp(item->label, "(") != 0) {
    printf("\t\t\tRESOLVING: %s\n", item->label);
    generate_code();
    rpar_semantic_action();
  }
  else {
    stack_pop(&operators_stack);
  }
}

void end_expr_semantic_action() {
  VariableStackItem *item = stack_lookup(&operators_stack);
  
  if (item == NULL) printf("\t\t\tendexpr: there is no operators\n");
  while (item != NULL && strcmp(item->label, "(") != 0) {
    printf("\t\t\tRESOLVING: %s\n", item->label);
    generate_code();
    item = stack_lookup(&operators_stack);
  }
}

void declaration_semantic_action(char *identifier) {
  VariableStackItem *item;
  VariableDescriptor *descriptor;
  char *label;
  
  // increment variable counter
  variables_counter++;
  // insert into symbol table and save its type (descriptor)
  label = generate_label(variables_counter, L_VARIABLE);
  descriptor = (VariableDescriptor *)malloc(sizeof(VariableDescriptor));
  descriptor->type = type_declared;
  descriptor->default_value = "0";
  
  sym_table_insert(identifier, label, descriptor);
  
  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  item->label = label;
  item->value = descriptor->default_value;
  
  stack_push(item, &variables_stack);
}

void remove_stack_variable_semantic_action() {
  stack_pop(&variables_stack);
  variables_counter--;
}

void number_semantic_action(char *number) {
  VariableStackItem *item;
  
  // increment the constants counter
  constants_counter++;
  // save the number value and associate it with the constant identifier label

  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  item->label = generate_label(constants_counter, L_CONSTANT);
  item->value = number;

  /* something like 
     C0 /2
     stack_push(&constants_stack, (token->value, constants_counter));
  */
  printf("! %s:%s pushed to stack\n", number, item->label);
  stack_push(item, &constants_stack);
  stack_push(item, &operands_stack);
}

void pop_lvalue_semantic_action() {
  VariableStackItem *item = stack_pop(&operands_stack);
  lvalue = item->label;
  printf("\t\t\tTrace: LVALUE setted to %s\n", lvalue);
}

void identifier_semantic_action(char *identifier) {
  VariableStackItem *item;
  SymTableEntry *entry;
  
  // push the identifier into the operands stack
  if (!is_identifier_declared(identifier))
    fatal_error("Identifier not declared.");
  
  entry = sym_table_get(identifier);
  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  item->label = entry->label; /* LABEL ASSOCIATED TO VARIABLE */
  item->value = NULL;

  printf("! %s:%s pushed to stack\n", identifier, item->label);
  stack_push(item, &operands_stack);
}

void lpar_semantic_action(char *symbol) {
  VariableStackItem *item;
  
  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  item->label = "("; /* ( */
  item->value = NULL;
  
  stack_push(item, &operators_stack);
}

/* STMT */

void if_semantic_action() {
  VariableStackItem *item = stack_pop(&operands_stack);
  VariableStackItem *else_label = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  VariableStackItem *endif_label =  (VariableStackItem *)malloc(sizeof(VariableStackItem));
  
  else_label->label = generate_label(++elses_counter, L_ELSE);
  endif_label->label = generate_label(++endifs_counter, L_ENDIF);

  stack_push(endif_label, &stmts_stack);
  stack_push(else_label, &stmts_stack);

  printf("\t\t\tTrace: Result from expression is %s\n", item->label);
  
  fprintf(fp, "          LD  %s\n", item->label);
  fprintf(fp, "          JZ  %s\n", else_label->label);
}

void else_semantic_action() {
  VariableStackItem *else_label = stack_pop(&stmts_stack);
  VariableStackItem *endif_label = stack_lookup(&stmts_stack);
  
  fprintf(fp, "          JP  %s\n", endif_label->label);
  fprintf(fp, "%-10s", else_label->label);
}

static int startswith(const char *a, const char *b) {
  const char *ptr = b;
  const char *ptr2 = a;
  for (;*ptr;) {
    if (*ptr2 == '\0') return 0;
    else if (*ptr++ != *ptr2++) return 0;
  }
  return 1;
}

void end_semantic_action() {
  VariableStackItem *top = stack_lookup(&stmts_stack);
  if (startswith(top->label, "ENDWHILE")) {
    VariableStackItem *endwhile_label = stack_pop(&stmts_stack);
    VariableStackItem *while_label = stack_pop(&stmts_stack);
    fprintf(fp, "          JP  %s\n", while_label->label);
    fprintf(fp, "%-10s", endwhile_label->label);
  } else {
    VariableStackItem *endif_label = stack_pop(&stmts_stack);
    fprintf(fp, "          JP  %s\n", endif_label->label);
    fprintf(fp, "%-10s", endif_label->label);
  }
}

void while_semantic_action() {
  VariableStackItem *while_label = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  VariableStackItem *endwhile_label =  (VariableStackItem *)malloc(sizeof(VariableStackItem));
  
  whiles_counter++;
  while_label->label = generate_label(whiles_counter, L_WHILE);
  endwhile_label->label = generate_label(whiles_counter, L_ENDWHILE);
  
  stack_push(while_label, &stmts_stack);
  stack_push(endwhile_label, &stmts_stack);
  
  fprintf(fp, "          JP  %s\n", while_label->label);
  fprintf(fp, "%-10s", while_label->label);
}

void stmt_expr_semantic_action() {
  VariableStackItem *result = stack_pop(&operands_stack);
  VariableStackItem *jump_label = stack_lookup(&stmts_stack);
  fprintf(fp, "          LD  %s\n", result->label);
  fprintf(fp, "          JZ  %s\n", jump_label->label);
}

void bool_operator_semantic_action(Token *token) {
  if (bool_operator != NULL) {
    fatal_error("Error: cannot have more than one boolean expression");
  }
  bool_operator = token;
  printf("SETTED: %s\n", bool_operator->value);
}

void bool_expr_semantic_action() {
  VariableStackItem *lexpr, *rexpr, *temp = NULL, *aux = NULL;
  char *ltrue, *lfalse;
  if (bool_operator == NULL) return; /* NOT AN BOOL EXPR */
  
  // lexpr < rexpr
  rexpr = stack_pop(&operands_stack);
  lexpr = stack_pop(&operands_stack);
  
  temps_counter++;
  temp = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  temp->label = generate_label(temps_counter, L_TEMP);
  temp->value = "0";
  bools_counter++;
  ltrue = generate_label(bools_counter, L_TRUE);
  lfalse = generate_label(bools_counter, L_FALSE);
  
  switch (bool_operator->class) {
    case EQ:
      // a == b
      //          LD #TEMP_0
      //          -  #TEMP_1
      //          JZ TRUE_0
      //          LV =0
      //          JP FALSE_0
      // TRUE_0   LV =1
      // FALSE_1  MM #TEMP_2
      fprintf(fp, "          LD  %s\n", lexpr->label);
      fprintf(fp, "          -   %s\n", rexpr->label);
      fprintf(fp, "          JZ  %s\n", ltrue);
      fprintf(fp, "          LD  FALSE\n");
      fprintf(fp, "          JP  %s\n", lfalse);
      fprintf(fp, "%-10sLD  TRUE\n", ltrue);
      fprintf(fp, "%-10sMM  %s\n", lfalse, temp->label);
      break;
    case NE:
      // a != b
      //          LD #TEMP_0
      //          -  #TEMP_1
      //          JZ FALSE_0
      //          LV =1
      //          JP TRUE_0
      // FALSE_0  LV =0
      // TRUE_1   MM #TEMP_2
      fprintf(fp, "          LD  %s\n", lexpr->label);
      fprintf(fp, "          -   %s\n", rexpr->label);
      fprintf(fp, "          JZ  %s\n", lfalse);
      fprintf(fp, "          LD  TRUE\n");
      fprintf(fp, "          JP  %s\n", ltrue);
      fprintf(fp, "%-10sLD  FALSE\n", lfalse);
      fprintf(fp, "%-10sMM  %s\n", ltrue, temp->label);
      break;
    case LT:
      // b < a (switch operands)
      aux = lexpr;
      lexpr = rexpr;
      rexpr = aux;
    case GT:
      // a > b
      //          LD #TEMP_0
      //          -  #TEMP_1
      //          JN FALSE_0
      //          JZ FALSE_0
      //          LD TRUE
      //          JP TRUE_0
      // FALSE_0  LD FALSE
      // TRUE_0   MM #TEMP_2
      fprintf(fp, "          LD  %s\n", lexpr->label);
      fprintf(fp, "          -   %s\n", rexpr->label);
      fprintf(fp, "          JN  %s\n", lfalse);
      fprintf(fp, "          JZ  %s\n", lfalse);
      fprintf(fp, "          LD  TRUE\n");
      fprintf(fp, "          JP  %s\n", ltrue);
      fprintf(fp, "%-10sLD  FALSE\n", lfalse);
      fprintf(fp, "%-10sMM  %s\n", ltrue, temp->label);
      break;
    case LE:
      // b <= b (switch operands)
      aux = lexpr;
      lexpr = rexpr;
      rexpr = aux;
    case GE:
      // a >= b
      //          LD #TEMP_0
      //          -  #TEMP_1
      //          JN FALSE_0
      //          LV =1
      //          JP TRUE_0
      // FALSE_0  LV =0
      // TRUE_0   MM #TEMP_2
      fprintf(fp, "          LD  %s\n", lexpr->label);
      fprintf(fp, "          -   %s\n", rexpr->label);
      fprintf(fp, "          JN  %s\n", lfalse);
      fprintf(fp, "          LD  TRUE\n");
      fprintf(fp, "          JP  %s\n", ltrue);
      fprintf(fp, "%-10sLD  FALSE\n", lfalse);
      fprintf(fp, "%-10sMM  %s\n", ltrue, temp->label);
      break;
    default:
      fatal_error("Error: bool operation not implmented");
  }
  
  /* register temp variable to be generated on data segment */
  stack_push(temp, &variables_stack);
  stack_push(temp, &operands_stack);
  bool_operator = NULL; /* RESET */
}

/* CODE GENERATION */

void generate_assignment_code() {
  VariableStackItem *item = NULL;
  
  if (lvalue == NULL) {
    printf("\t\t\tWarning: not an assignment expression it is useless.\n");
    return;
  }
    
  /* FINAL CONDITION: OPERANDS CONTAINS THE EXPR RESULT */
  item = stack_pop(&operands_stack);
  if (item == NULL) printf("ASSIGNGEN: NO operands\n");
  else
    printf("\t\t\t-----------------\n\t\t\t%s = (%s, %s)\n\t\t\t-----------------\n", lvalue, item->label, item->value);

  fprintf(fp, "          LD  %s\n", item->label);
  fprintf(fp, "          MM  %s\n", lvalue);
  
  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  item->label = lvalue;
  item->value = NULL;
  
  stack_push(item, &operands_stack);
  /* PRE-CONDITION, while assign not found lvalue is NULL */
  lvalue = NULL;
}

void generate_end_program_code() {
  fprintf(fp, "          HM /0\n");
}

void generate_data_definition_code() {
  fprintf(fp, "          @   /200\n");
  while (!stack_empty(&variables_stack)) {
    VariableStackItem *item = (VariableStackItem *)stack_pop(&variables_stack);
    fprintf(fp, "%-10sK   =%s\n", item->label, item->value);
  }
  
  while (!stack_empty(&constants_stack)) {
    VariableStackItem *item = (VariableStackItem *)stack_pop(&constants_stack);
    fprintf(fp, "%-10sK   =%s\n", item->label, item->value);
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
  stack_clean(&stmts_stack);
}