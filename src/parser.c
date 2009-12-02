/*
  ALGORITH:
  - condicao inicial: tenho um token lido
  - para o nó raiz: (unico)
  - se token eh NULL, so considero meu programa valido se
  estou em estado final
  - caso contrario meu programa esta errado
  * para as demais maquinas, NULL eh considerado erro
  - invariant: sempre leio token no final de toda submaquina
  - no estado inicial da maquina, se nao consome nenhum token,
  eh suposto que a sintaxe esta invalida, exceto para nó raiz
  - nos estados:
  - nas transicoes:
  - se consome token, atualizo o current_state
  - se chamada de submaquina, atualizo o current_state e 
  uso continue; (para nao consumir token), se falhar, 
  retorno 0 (erro)
  - se for default
  - estou num estado final, retorno 1 (sintaxe ok)
  - caso contrario, retorno 0 (sintaxe invalida, tratar?)
*/

#include <stdio.h>
#include "parser.h"
#include "string.h"

static Token *token = NULL;

extern Stack operators_stack;
extern Stack operands_stack;
extern Stack variables_stack;
extern Stack constants_stack;
extern class_t type_declared;
extern int variables_counter;
extern int constants_counter;
extern int ifs_counter;
extern int elses_counter;
extern int endifs_counter;
extern char* lvalue;

int parse(BufferedInputStream *source_code_stream) {
  semantic_initialize();
  printf("Start parsing...\n");
  token = next_token(source_code_stream);
  if (!is_prog(source_code_stream)) {
    return 0;
  }
  
  printf("program OK\n");
  generate_end_program_code();
  generate_data_definition_code();
  
  return 1;
}

int is_prog(BufferedInputStream *source_code_stream) {
  while (1) {
    if (token == NULL)
      return 1;
    
    if (is_external_decl(source_code_stream))
      continue;
    else
      return 0;
  }
}

int is_external_decl(BufferedInputStream *source_code_stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return current_state == 6 ? 1 : 0;

    //printf("External Declaration, state: <%d>, token: <%s>\n", current_state, token->value); 

    switch (current_state) {
    case 0:
      if (token->class == INT || token->class == VOID) {
        type_declared = token->class;
        current_state = 1;
      }
      else
        return 0;
      break;
    case 1:
      if (token->class == IDENTIFIER) {
        declaration_semantic_action(token->value);
        current_state = 2;
      }
      else if (token->class == OPERATION) {
        declaration_semantic_action(token->value);
        current_state = 3;
      }
      else
        return 0;
      break;
    case 2:
      switch (token->class) {
      case ASSIGN:
        current_state = 5;
        break;
      case LPAR:
        routine_definition_semantic_action();
        begin_scope_semantic_action();
        current_state = 4;
        break;
      case LBRA:
        current_state = 7;
        break;
      case SEMICOLON:
        /* var declaration without value */
        decl_variable_semantic_action();
        current_state = 6;
        break;
      default:
        return 0;
      }
      break;
    case 3:
      if (token->class == LPAR) {
        routine_definition_semantic_action();
        begin_scope_semantic_action();
        current_state = 4;
      }
      else
        return 0;
      break;
    case 4:
      if (token->class == INT || token->class == VOID) {
        type_declared = token->class;
        current_state = 8;
      }
      else if (token->class == RPAR) {
        end_declaration_semantic_action();
        current_state = 9;
      }
      else
        return 0;
      break;
    case 5:
      if (is_expr(source_code_stream)) {
        current_state = 19;
        continue;
      }
      else return 0;
      break;
    case 6:
      end_declaration_semantic_action();
      return 1;
    case 7:
      if (token->class == NUMBER)
        current_state = 10;
      else return 0;
      break;
    case 8:
      if (token->class == IDENTIFIER) {
        routine_param_semantic_action(token->value);
        current_state = 16;
      }
      else return 0;
      break;
    case 9:
      if (token->class == DO) {
        create_routine_semantic_action();
        current_state = 11;
      }
      else
        return 0;
      break;
    case 10:
      if (token->class == RBRA)
        current_state = 12;
      else
        return 0;
      break;
    case 11:
      if (is_compound_stmt(source_code_stream)) {
        current_state = 13;
        continue;
      }
      else return 0;
      break;
    case 12:
      /* ENDED DECLARATION */
      if (token->class == SEMICOLON)
        current_state = 6;
      else if (token->class == ASSIGN)
        current_state = 14;
      else return 0;
      break;
    case 13:
      if (token->class == END) {
        end_routine_semantic_action();
        end_scope_semantic_action();
        current_state = 6;
      }
      else return 0;
      break;
    case 14:
      if (token->class == LCUR)
        current_state = 15;
      else return 0;
      break;
    case 15:
      if (token->class == NUMBER)
        current_state = 17;
      else return 0;
      break;
    case 16:        
      if (token->class == COMMA) {
        current_state = 18;
      }
      else if (token->class == RPAR) {
        end_declaration_semantic_action();
        current_state = 9;
      }
      else return 0;
      break;
    case 17:
      if (token->class == COMMA)
        current_state = 15;
      else if (token->class == RCUR)
        current_state = 19;
      else return 0;
      break;
    case 18:
      if (token->class == INT || token->class == VOID) {
        type_declared = token->class;
        current_state = 8;
      }
      else return 0;
      break;
    case 19:
      if (token->class == SEMICOLON) {
        end_expr_semantic_action();
        /* var declaration with value */
        decl_variable_semantic_action();
        current_state = 6;
      }
      else return 0;
      break;
    }
    token = next_token(source_code_stream);
  }
}

int is_decl(BufferedInputStream *source_code_stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return current_state == 4 ? 1 : 0;

    //printf("Declaration, state: <%d>, token: <%s>\n", current_state, token->value); 

    switch (current_state) {
    case 0:
      if (token->class == INT || token->class == VOID) {
        type_declared = token->class;
        current_state = 1;
      }
      else return 0;
      break;
    case 1:
      if (token->class == IDENTIFIER) {
        declaration_semantic_action(token->value);
        current_state = 2;
      }
      else return 0;
      break;
    case 2:
      if (token->class == ASSIGN)
        current_state = 3;
      else if (token->class == SEMICOLON) {
        /* WITHOUT VALUE */
        decl_variable_semantic_action();
        current_state = 4;
      }
      else if (token->class == LBRA)
        current_state = 5;
      else return 0;
      break;
    case 3:
      if (is_expr(source_code_stream)) {
        current_state = 6;
        continue;
      }
      else
        return 0;
      break;
    case 4:
      end_declaration_semantic_action();
      return 1;
    case 5:
      if (token->class == NUMBER)
        current_state = 7;
      else return 0;
      break;
    case 6:
      if (token->class == SEMICOLON) {
        /* WITH VALUE */
        end_expr_semantic_action();
        decl_variable_semantic_action();
        current_state = 4;
      }
      else return 0;
      break;
    case 7:
      if (token->class == RBRA)
        current_state = 8;
      else return 0;
      break;
    case 8:
      if (token->class == ASSIGN) {
        current_state = 9;
      }
      else if (token->class == SEMICOLON) {
        current_state = 4;
      }
      else return 0;
      break;
    case 9:
      if (token->class == LCUR)
        current_state = 10;
      else return 0;
      break;
    case 10:
      if (token->class == NUMBER)
        current_state = 11;
      else return 0;
      break;
    case 11:
      if (token->class == COMMA)
        current_state = 10;
      else if (token->class == RCUR) {
        current_state = 6;
      }
      else return 0;
      break;
    }
    token = next_token(source_code_stream);
  }
}

int is_compound_stmt(BufferedInputStream *source_code_stream) {
  int current_state = 0;

  while (1) { 
    if (token == NULL) 
      return 1;

    //printf("Compound Stmt, state: <%d>, token: <%s>\n", current_state, token->value); 

    switch (current_state) {
    case 0:
      if (is_decl(source_code_stream)) {
        current_state = 0;
        continue;
      }
      else if (is_stmt(source_code_stream)) {
        current_state = 1;
        continue;
      }
      else
        return 1;
      break;
    case 1:
      if (is_stmt(source_code_stream)) {
        current_state = 1;
        continue;
      }
      else return 1;
      break;
    }
    token = next_token(source_code_stream);
  }
}

int is_stmt(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return 0;
    
    //printf("Statement, state: <%d>, token: <%s>\n", current_state, token->value); 
    
    switch (current_state) {
    case 0:
      if (token->class == IF) {
        current_state = 1;
      }
      else if (token->class == WHILE) {
        while_semantic_action();
        current_state = 3;
      }
      else if (token->class == RETURN) {
        register_return_routine_semantic_action();
        current_state = 4;
      }
      else if (token->class == SCAN || token->class == PRINT)
        current_state = 5;
      else if (is_expr(stream)) {
        current_state = 2;
        continue;
      }
      else return 0;
      break;
    case 1:
      if (token->class == LPAR) {
        current_state = 6;
      }
      else return 0;
      break;
    case 2:
      if (token->class == SEMICOLON) {
        end_expr_semantic_action();
        /* RESOLVE IF BOOL EXPR */
        bool_expr_semantic_action();
        generate_assignment_code();
        return_routine_semantic_action(1);
        current_state = 12;
      }
      else return 0;
      break;
    case 3:
      if (token->class == LPAR)
        current_state = 16;
      else return 0;
      break;
    case 4:
      if (token->class == SEMICOLON) {
        return_routine_semantic_action(0);
        current_state = 12;
      }
      else if (is_expr(stream)) {
        current_state = 2;
        continue;
      }
      else return 0;
      break;
    case 5:
      if (token->class == LPAR) {
        current_state = 7;
      }
      else return 0;
      break;
    case 6:
      if (is_expr(stream)) {
        /* IF STMT EXPR */
        end_expr_semantic_action();
        /* RESOLVE IF BOOL EXPR */
        bool_expr_semantic_action();
        /* RESOLVE IF ASSIGN EXPR */
        generate_assignment_code();
        if_semantic_action();
        current_state = 8;
        continue;
      }
      else return 0;
      break;
    case 7:
      if (is_expr(stream)) {
        current_state = 9;
        continue;
      }
      else return 0;
      break;
    case 8:
      if (token->class == RPAR) {
        current_state = 10;
      }
      else return 0;
      break;
    case 9:
      if (token->class == RPAR) {
        current_state = 2;
      }
      else return 0;
      break;
    case 10:
      if (token->class == DO) {
        current_state = 11;
      }
      else return 0;
      break;
    case 11:
      if (is_compound_stmt(stream)) {
        current_state = 13;
        continue;
      }
      else return 0;
      break;
    case 12:
      return 1;
    case 13:
      if (token->class == ELSE) {
        else_semantic_action();
        current_state = 14;
      }
      else if (token->class == END)
        current_state = 12;
      else return 0;
      break;
    case 14:
      if (is_compound_stmt(stream)) {
        current_state = 15;
        continue;
      }
      else return 0;
      break;
    case 15:
      if (token->class == END) {
        end_semantic_action();
        current_state = 12;
      }
      else return 0;
      break;
    case 16:
      if (is_expr(stream)) {
        /* WHILE STMT EXPR */
        end_expr_semantic_action();
        /* RESOLVE IF BOOL EXPR */
        bool_expr_semantic_action();
        /* RESOLVE IF ASSIGN EXPR */
        generate_assignment_code();
        stmt_expr_semantic_action();
        current_state = 17;
        continue;
      }
      else return 0;
      break;
    case 17:
      if (token->class == RPAR) {
        current_state = 18;
      }
      else return 0;
      break;
    case 18:
      if (token->class == DO) {
        current_state = 14;
      }
      else return 0;
      break;
    }
    token = next_token(stream);
  }
}

int is_expr(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return 0; /* ERROR: NOT ROOT NODE */

    //printf("Expression, state: <%d>, token: <%s>\n", current_state, token->value);  

    switch (current_state) {
      case 0:
      if (is_factor(stream)) {
        current_state = 1;
        continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 1:
      switch (token->class) {
      case MULT:
      case DIV:
        mult_div_semantic_action(token->value);
        current_state = 2;
        break;
      case ADD:
      case SUB:
        add_sub_semantic_action(token->value);
        current_state = 2;
        break;
      case LT:
      case GT:
      case GE:
      case LE:
      case EQ:
      case NE:
        /* EVALUATES THE LEFT EXPR */
        end_expr_semantic_action();
        bool_operator_semantic_action(token);
        current_state = 3;
        break;
      case ASSIGN:
        pop_lvalue_semantic_action();
        current_state = 4;
        break;
      default:
        //end_expr_semantic_action();
        return 1; /* ACCEPT: FINAL STATE */
      }
      break;
    case 2:
      if (is_factor(stream)) {
        current_state = 5;
        continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 3:
      if (is_factor(stream)) {
        current_state = 7;
        continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
      }
      else return 0; /* ERROR: NOT FINAL STATE */
    case 4:
      if (is_expr(stream)) {
        current_state = 6;
        continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
      }
      else return 0; /* ERROR: NOT FINAL STATE */
    case 5:
      switch (token->class) {
      case MULT:
      case DIV:
        mult_div_semantic_action(token->value);
        current_state = 2;
        break;
      case ADD:
      case SUB:
        add_sub_semantic_action(token->value);
        current_state = 2;
        break;
      case LT:
      case GT:
      case GE:
      case LE:
      case EQ:
      case NE:
        current_state = 3;
        break;
      default:
        return 1; /* ACCEPT: FINAL STATE */
      }
      break;
    case 6:
      return 1;
    case 7:
      switch (token->class) {
        case MULT:
        case DIV:
          mult_div_semantic_action(token->value);
          current_state = 3;
          break;
        case ADD:
        case SUB:
          add_sub_semantic_action(token->value);
          current_state = 3;
          break;
        default:
          return 1;
      }
    }
    /* INVARIANT: always consume token */
    token = next_token(stream);
  }
}

int is_factor(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return 0; /* ERROR: NOT ROOT TREE */
      
    //printf("Factor, state: <%d>, token: <%s>\n", current_state, token->value);  

    switch (current_state) {
    case 0:
      switch (token->class) {
      case NUMBER:
        number_semantic_action(token->value);
        current_state = 1;
        break;
      case IDENTIFIER:
        identifier_semantic_action(token->value);
        current_state = 2;
        break;
      case OPERATION:
        identifier_semantic_action(token->value);
        current_state = 3;
        break;
      case LPAR:
        lpar_semantic_action();
        current_state = 4;
        break;
      default: /* ERROR: NOT FINAL STATE */
        return 0;
      }
      break;
    case 1:
      return 1; /* ACCEPT: FINAL STATE */
    case 2:
      if (token->class == LBRA)
        current_state = 6;
      else if (token->class == LPAR) {
        /* FUNCTION CALL */
        pop_fname_semantic_action();
        prepare_call_routine_semantic_action();
        current_state = 7;
      }
      else return 1; /* ACCEPT: FINAL STATE */
      break;
    case 3:
      if (token->class == LPAR) {
        /* FUNCTION CALL */
          
        current_state = 7;
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 4:
      if (is_expr(stream)) {
        current_state = 5;
        continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 5:
      if (token->class == RPAR) {
        rpar_semantic_action();
        current_state = 1;
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 6:
      if (token->class == NUMBER)
        current_state = 10;
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 7:
      if (token->class == RPAR) {
        call_routine_semantic_action();
        current_state = 1;
      }
      else if (is_expr(stream)) {
        current_state = 8;
        continue;
      }
      else return 0;
      break;
    case 8:
      if (token->class == COMMA) {
        end_expr_semantic_action();
        bool_expr_semantic_action();
        generate_assignment_code();
        push_arg_routine_semantic_action();
        current_state = 9;
      }
      else if (token->class == RPAR) {
        /* END FUNCTION PARAMS */
        end_expr_semantic_action();
        bool_expr_semantic_action();
        generate_assignment_code();
        push_arg_routine_semantic_action();
        call_routine_semantic_action();
        current_state = 1;
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 9:
      if (is_expr(stream)) {
        current_state = 8;
        continue;
      }
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    case 10:
      if (token->class == RBRA)
        current_state = 1;
      else return 0; /* ERROR: NOT FINAL STATE */
      break;
    }
    /* INVARIANT: always consume token */
    token = next_token(stream);
  }
}
