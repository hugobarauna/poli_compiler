#include "semantic_actions.h"
#include "lexer.h"
#include "scope.h"

class_t type_declared;
int variables_counter = -1;
int constants_counter = -1;
int ifs_counter = -1;
int elses_counter = -1;
int endifs_counter = -1;
int whiles_counter = -1;
int bools_counter = -1;
int temps_counter = -1;
int funcs_counter = -1;
int func_vars_counter = -1;
int returns_counter = -1;
int is_return = 0;
Hashtable *sym_table;
Stack operators_stack, operands_stack, constants_stack, variables_stack;
Stack routine_args_stack;
int call_routine_arg_pos = 0;
Stack stmts_stack;
Token *bool_operator = NULL;
char *lvalue = NULL;
char *fname = NULL;
RoutineDescriptor *func = NULL;
RoutineDescriptor *call_func = NULL;

static void clean_stacks();

static FILE *fp = NULL;

static VariableStackItem *stack_item_new(char *label, char *value, char *comment) {
  VariableStackItem *item;
  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  if (item == NULL)
    fatal_error("Error: couldn't create a VariableStackItem");
  
  item->label = label;
  item->value = value;
  item->comment = comment;

  return item;
}

void semantic_initialize() {
  VariableStackItem *ktrue, *kfalse;
  scope_reset();
  scope_new(); /* GLOBAL SCOPE */
  
  clean_stacks();
  /* LOAD FIXED CONSTANTS */  
  ktrue = stack_item_new("TRUE", "1", "true bool");
  kfalse = stack_item_new("FALSE", "0", "false bool");
  
  stack_push(ktrue, &constants_stack);
  stack_push(kfalse, &constants_stack);
  
  fp = fopen("out.asm", "w");
  fprintf(fp, "        @  /0\n");
  fprintf(fp, "        SC main\n");
  fprintf(fp, "        HM /0\n");
  /* Maybe here, I already can open the output .asm file and write the first headers,
     somethin like this
           @ /0
  */
}

char* generate_label(int counter, label_t type) {
  char* label = (char* ) malloc(20);
  if (label == NULL)
    fatal_error("Failed: Couln't alloc space for label.");

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
    case L_FUNC_VAR:
      strcpy(label, "FUNC_VAR_");
      break;
    case L_FUNC:
      strcpy(label, "FUNC_");
      break;
    case L_FUNC_RESULT:
      strcpy(label, "FUNC_RESULT_");
      break;
    case L_FUNC_END:
      strcpy(label, "FUNC_END_");
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
  char *label;

  operator = stack_pop(&operators_stack);
  rvalue = stack_pop(&operands_stack);
  lvalue = stack_pop(&operands_stack);
  
  temps_counter++;
  label = generate_label(temps_counter, L_TEMP);
  temp = stack_item_new(label, "0", "");
  
  fprintf(fp, "          LD  %s\n", lvalue->label);
  fprintf(fp, "          %-2s  %s\n", operator->label, rvalue->label);
  fprintf(fp, "          MM  %s\n", temp->label);
  
//  printf("\t\t\t-----------------\n\t\t\t%s = %s %s %s\n\t\t\t-----------------\n", 
//    temp->label, lvalue->label, operator->label, rvalue->label);

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
    item = stack_item_new(operator, NULL, "");
//    printf("! %s pushed to stack\n", item->label);
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
    item = stack_item_new(operator, NULL, "");
//    printf("! %s pushed to stack\n", item->label);
    stack_push(item, &operators_stack);
  }
}

void rpar_semantic_action() {
  VariableStackItem *item = stack_lookup(&operators_stack);
//  printf("~rpar_semantic_action\n");
  if (item != NULL && strcmp(item->label, "(") != 0) {
//    printf("\t\t\tRESOLVING: %s\n", item->label);
    generate_code();
    rpar_semantic_action();
  }
  else {
    stack_pop(&operators_stack);
  }
}

void end_expr_semantic_action() {
//  printf("~end_expr_semantic_action\n");
  VariableStackItem *item = stack_lookup(&operators_stack);
  if (item == NULL) return;
  
  while (item != NULL && strcmp(item->label, "(") != 0) {
    generate_code();
    item = stack_lookup(&operators_stack);
  }
}

void declaration_semantic_action(char *identifier) {
//  printf("~rpar_semantic_action\n");
  if (lvalue != NULL)
    fatal_error("Error: Cannot have more than one declaration");
  lvalue = identifier;
}

void decl_variable_semantic_action() {
  VariableStackItem *item;
  VariableDescriptor *descriptor;
  char *label;
//  printf("~decl_variable_semantic_action\n");
  
  if (is_identifier_locally_declared(lvalue))
    fatal_error("Error: Identifier already declared.");
  
  // increment variable counter
  variables_counter++;
  // insert into symbol table and save its type (descriptor)
  label = generate_label(variables_counter, L_VARIABLE);
  descriptor = (VariableDescriptor *)malloc(sizeof(VariableDescriptor));
  descriptor->type = type_declared;
  descriptor->name = lvalue; /* GET NAME */
  descriptor->default_value = "0";
  
  if (!stack_empty(&operands_stack)) {
    VariableStackItem *result = stack_pop(&operands_stack);
  
    fprintf(fp, "    LD %s\n", result->label);
    fprintf(fp, "    MM %s\n", label);
  }

  scope_sym_table_insert(lvalue, label, descriptor);
  item = stack_item_new(label, descriptor->default_value, lvalue);
  
  /* register variable name */
  stack_push(item, &variables_stack);
}


void routine_definition_semantic_action() {
//  printf("~routine_semantic_action\n");
  if (func != NULL)
    fatal_error("Error: Cannot have a function inside a function\n");

  RoutineDescriptor *descriptor;

  if (is_identifier_locally_declared(lvalue))
    fatal_error("Error: conflict name.");

  // insert into symbol table and save its type (descriptor)
  returns_counter++;
  descriptor = (RoutineDescriptor *)malloc(sizeof(RoutineDescriptor));
  descriptor->return_type = type_declared;
  descriptor->name = lvalue;
  descriptor->num_params = 0;
  descriptor->result = generate_label(returns_counter, L_FUNC_RESULT);
  descriptor->end = generate_label(returns_counter, L_FUNC_END);
  func = descriptor;
  
  scope_sym_table_insert(lvalue, lvalue, descriptor);
}

void routine_param_semantic_action(char *identifier) {
  VariableDescriptor *variable;
  char *label;
  
//  printf("~routine_param_semantic_action\n");
  
  if (is_identifier_locally_declared(identifier))
    fatal_error("Error: Variable name conflict in function declaration");
  
  // increment variable counter
  func_vars_counter++;
  // insert into symbol table and save its type (descriptor)
  label = generate_label(func_vars_counter, L_FUNC_VAR);
  variable = (VariableDescriptor *)malloc(sizeof(VariableDescriptor));
  variable->type = type_declared;
  variable->name = label;
  variable->default_value = "0";
  
  scope_sym_table_insert(identifier, label, variable);
  
  func->params[func->num_params] = variable;
  func->num_params++;
}

void create_routine_semantic_action() {
  int i;
//  printf("~create_routine_semantic_action\n");
  for (i = 0; i < func->num_params; ++i)
    fprintf(fp, "%s  K  =0\n", func->params[i]->name);
  fprintf(fp, "%s  K  /0\n", func->result);
  fprintf(fp, "%s  K  /0\n", func->name);
}

void register_return_routine_semantic_action() {
//  printf("~register_return_routine_semantic_action\n");
  is_return = 1;
}

void return_routine_semantic_action(int has_param) {
//  printf("~return_routine_semantic_action\n");
  if (!is_return) return;
  if (has_param && func->return_type == VOID)
    fatal_error("Error: Return expression does not return a value");

  if (func->return_type == VOID)
    fprintf(fp, "         RS %s\n", func->name);
  else {
    // LOAD VARIABLE
    if (stack_empty(&operands_stack)) printf("EMPTY!\n");
    VariableStackItem *item = stack_pop(&operands_stack);
    fprintf(fp, "         LD %s\n", item->label);
    fprintf(fp, "         MM %s\n", func->result);
    fprintf(fp, "         JP %s\n", func->end);
  }
  is_return = 0;
}

void end_routine_semantic_action() {
//  printf("~end_routine_semantic_action\n");
  fprintf(fp, "%s RS  %s\n", func->end, func->name);
  func = NULL;
}

void begin_scope_semantic_action() {
//  printf("~begin_semantic_action\n");
  scope_new();
}

void end_scope_semantic_action() {
//  printf("~end_scope_semantic_action\n");
  scope_remove();
}

void end_declaration_semantic_action() {
//  printf("~end_declaration_semantic_action\n");
  /* RESET */
  lvalue = NULL;
}

void remove_stack_variable_semantic_action() {
//  printf("~remove_stack_variable_semantic_action\n");
  stack_pop(&variables_stack);
  variables_counter--;
}

void number_semantic_action(char *number) {
  VariableStackItem *item;
  char *k;
  
//  printf("~number_semantic_action\n");
  
  // increment the constants counter
  constants_counter++;
  // save the number value and associate it with the constant identifier label
  k = generate_label(constants_counter, L_CONSTANT);
  item = stack_item_new(k, number, k);

  /* something like 
     C0 /2
     stack_push(&constants_stack, (token->value, constants_counter));
  */
//  printf("! %s:%s pushed to stack\n", number, item->label);
  stack_push(item, &constants_stack);
  stack_push(item, &operands_stack);
}

void pop_lvalue_semantic_action() {
  VariableStackItem *item = stack_pop(&operands_stack);
//  printf("~pop_lvalue_semantic_action\n");
  lvalue = item->label;
  //printf("\t\t\tTrace: LVALUE setted to %s\n", lvalue);
}

void pop_fname_semantic_action() {
  VariableStackItem *item = stack_pop(&operands_stack);
//  printf("~pop_fname_semantic_action\n");
  fname = item->label;
  //printf("\t\t\tTrace: FNAME setted to %s\n", fname);
}

void push_arg_routine_semantic_action() {
  VariableStackItem *item;
//  printf("~push_arg_routine_semantic_action\n");
  if (call_routine_arg_pos > call_func->num_params)
    fatal_error("Error: Exceeded the number of args");
  item = stack_pop(&operands_stack);
  fprintf(fp, "           LD %s\n", item->label);
  fprintf(fp, "           MM %s\n", call_func->params[call_routine_arg_pos]->name);
  call_routine_arg_pos++;
}

void prepare_call_routine_semantic_action() {
  SymTableEntry *entry = scope_sym_table_get(fname);
//  printf("~prepare_call_routine_semantic_action\n");
  call_func = (RoutineDescriptor *)entry->descriptor;
//  printf("LOADED FUNCTION %s\n", call_func->name);
}

void call_routine_semantic_action() {
  VariableStackItem *item;
//  printf("~call_routine_semantic_action\n");
  if (call_routine_arg_pos+1 < call_func->num_params)
    fatal_error("Error: Missing function arguments");
  fprintf(fp, "        SC  %s\n", call_func->name);

//  printf("PUSHED %s\n", call_func->result);
  item = stack_item_new(call_func->result, NULL, call_func->name);
  stack_push(item, &operands_stack);
  
  /* RESET */
  call_routine_arg_pos = 0;
  call_func = NULL;
  fname = NULL;
}

void identifier_semantic_action(char *identifier) {
  VariableStackItem *item;
  SymTableEntry *entry;
  
//  printf("~identifier_semantic_action\n");
  
  // push the identifier into the operands stack
  if (!is_identifier_declared(identifier))
    fatal_error("Error: Variable not declared");
  
  entry = scope_sym_table_get(identifier);
  item = stack_item_new(entry->label, NULL, identifier);

//  printf("! %s:%s pushed to stack\n", identifier, item->label);
  stack_push(item, &operands_stack);
}

void lpar_semantic_action(char *symbol) {
  VariableStackItem *item;
  
//  printf("~lpar_semantic_action\n");
  
  item = (VariableStackItem *)malloc(sizeof(VariableStackItem));
  item->label = "("; /* ( */
  item->value = NULL;
  item->comment = "(";
  
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

//  printf("\t\t\tTrace: Result from expression is %s\n", item->label);
  
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
//  printf("~stmt_expr_semantic_action\n");
  VariableStackItem *result = stack_pop(&operands_stack);
  VariableStackItem *jump_label = stack_lookup(&stmts_stack);
  fprintf(fp, "          LD  %s\n", result->label);
  fprintf(fp, "          JZ  %s\n", jump_label->label);
}

void bool_operator_semantic_action(Token *token) {
//  printf("~bool_operator_semantic_action\n");
  if (bool_operator != NULL) {
    fatal_error("Error: cannot have more than one boolean expression");
  }
  bool_operator = token;
  //printf("SETTED: %s\n", bool_operator->value);
}

void bool_expr_semantic_action() {
//  printf("~bool_expr_semantic_action\n");
  VariableStackItem *lexpr, *rexpr, *temp = NULL, *aux = NULL;
  char *ltrue, *lfalse, *label;
  if (bool_operator == NULL) return; /* NOT AN BOOL EXPR */
  
  // lexpr < rexpr
  rexpr = stack_pop(&operands_stack);
  lexpr = stack_pop(&operands_stack);
  
  temps_counter++;
  label = generate_label(temps_counter, L_TEMP);
  temp = stack_item_new(label, "0", label);
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
//  printf("~generate_assignment_code\n");
  VariableStackItem *item = NULL;
  
  if (lvalue == NULL) {
//    printf("\t\t\tWarning: not an assignment expression it is useless.\n");
    return;
  }
    
  /* FINAL CONDITION: OPERANDS CONTAINS THE EXPR RESULT */
  item = stack_pop(&operands_stack);
//  if (item == NULL) printf("ASSIGNGEN: NO operands\n");
//  else
//    printf("\t\t\t-----------------\n\t\t\t%s = (%s, %s)\n\t\t\t-----------------\n", lvalue, item->label, item->value);

  fprintf(fp, "          LD  %s\n", item->label);
  fprintf(fp, "          MM  %s\n", lvalue);
  
  item = stack_item_new(lvalue, NULL, lvalue);
  
  stack_push(item, &operands_stack);
  /* PRE-CONDITION, while assign not found lvalue is NULL */
  lvalue = NULL;
}

void generate_end_program_code() {
//  printf("~generate_end_semantic_action\n");
  fprintf(fp, "          HM /0\n");
}

void generate_data_definition_code() {
//  printf("generate_data_semantic_action\n");
  fprintf(fp, "          @   /200\n");
  while (!stack_empty(&variables_stack)) {
    VariableStackItem *item = (VariableStackItem *)stack_pop(&variables_stack);
    fprintf(fp, "%-15sK   =%s ; %s\n", item->label, item->value, item->comment);
  }
  
  while (!stack_empty(&constants_stack)) {
    VariableStackItem *item = (VariableStackItem *)stack_pop(&constants_stack);
    fprintf(fp, "%-10sK   =%s ; \n", item->label, item->value);
  }
  fclose(fp);
}

static void clean_stacks() {
  stack_clean(&operators_stack);
  operators_stack.name = "OPERATORS";
  
  stack_clean(&operands_stack);
  operands_stack.name = "OPERANDS";
  
  stack_clean(&constants_stack);
  constants_stack.name = "CONSTANTS";
  
  stack_clean(&variables_stack);
  variables_stack.name = "VARIABLES";
  
  stack_clean(&stmts_stack);
  stmts_stack.name = "STMTS";
  
  stack_clean(&routine_args_stack);
  routine_args_stack.name = "ROUTINE_ARGS";
}