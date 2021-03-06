#ifndef __SEMANTIC_ACTIONS_H__
#define __SEMANTIC_ACTIONS_H__

#include <string.h>

#include "hashtable.h"
#include "stack.h"
#include "lexer.h"
#include "xstring.h"
#include "error.h"
#include "scope.h"

typedef struct _variable_stack_item {
  char *label;
  char *value;
  char *comment;
} VariableStackItem;

typedef enum e_label_type {
  L_VARIABLE,
  L_CONSTANT,
  L_TEMP,
  L_IF,
  L_ELSE,
  L_ENDIF,
  L_WHILE,
  L_ENDWHILE,
  L_FUNC_VAR,
  L_FUNC_RESULT,
  L_FUNC_END,
  L_FUNC,
  L_TRUE,
  L_FALSE
} label_t;

void semantic_initialize();

char* generate_label(int counter, label_t type);

/* SEMANTIC ACTIONS */

void while_semantic_action();
void if_semantic_action();
void else_semantic_action();
void end_semantic_action();
void stmt_expr_semantic_action();

void declaration_semantic_action(char *identifier);
void decl_variable_semantic_action();
void end_declaration_semantic_action();
void remove_stack_variable_semantic_action();
void pop_lvalue_semantic_action();

void number_semantic_action(char *);
void identifier_semantic_action(char *);

void lpar_semantic_action();
void rpar_semantic_action();

void mult_div_semantic_action(char *operator);
void add_sub_semantic_action(char *operator);

void end_expr_semantic_action();

void bool_operator_semantic_action(Token *token);
void bool_expr_semantic_action();

void pop_fname_semantic_action();
void routine_semantic_action();
void routine_definition_semantic_action();
void create_routine_semantic_action();
void end_routine_semantic_action();
void return_routine_semantic_action();
void prepare_call_routine_semantic_action();
void call_routine_semantic_action();
void push_arg_routine_semantic_action();
void register_return_routine_semantic_action();
void routine_param_semantic_action(char *identifier);
void begin_scope_semantic_action();
void end_scope_semantic_action();

void register_io_semantic_action(class_t io_type);
void io_semantic_action();

/* CODE GENERATION */

void generate_code();
void generate_assignment_code();
void generate_end_program_code();
void generate_data_definition_code();

#endif