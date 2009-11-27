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
#include "lexer.h"

static Token operators[1000];
static Token operands[1000];
static int operator_sp = 0;
static int operand_sp = 0;

/* STACKS */
#define push_operator(v) (operators[operator_sp++] = (v))
#define pop_operator  (operators[--operator_sp])

#define push_operand(v) (operand[operand_sp++] = (v))
#define pop_operand  (operand[--operand_sp])

static Token *token = NULL;

int parse(BufferedInputStream *source_code_stream) {
  token = next_token(source_code_stream);
  return is_prog(source_code_stream);
  //return is_assignment(source_code_stream);
}

int is_prog(BufferedInputStream *stream) {
  while (1) {
    if (token == NULL)
      return 1;
    
    if (is_decl(stream))
      continue;
    else
      return 0;
  }
}

int is_decl(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return current_state == 2 ? 1 : 0;

    printf("Declaration, state: <%d>, token: <%s>\n", current_state, token->value); 

    switch (current_state) {
      case 0:
        if (token->class == INT) {
          // register_type_decl
          current_state = 1;
        }
        else if (is_assignment(stream)) {
          current_state = 2;
          continue;
        }
        else
          return 0;
        break;
      case 1:
        if (token->class == IDENTIFIER) {
          // insert into symbol table and save its type (descriptor)
          // increment variable counter
          current_state = 3;
        }
        else
          return 0;
        break;
      case 2:
        return 1;
      case 3:
        if (token->class == SEMICOLON)
          current_state = 2;
        else
          return 0;
        break;
    }
    token = next_token(stream);
  }
}

int is_stmt(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return 0;
    
    printf("Statement, state: <%d>, token: <%s>\n", current_state, token->value); 
    
    switch (current_state) {
      case 0:
        if (token->class == IF)
          current_state = 1;
        else if (token->class == WHILE)
          current_state = 2;
        else if (token->class == RETURN)
          current_state = 3;
        else if (is_assignment(stream)) {
          current_state = 4;
          continue;
        }
        else
          return 0;
        break;
      case 1:
        if (token->class == LPAR)
          current_state = 5;
        else
          return 0;
        break;
      case 2:
        if (token->class == LPAR)
          current_state = 9;
        else
          return 0;
        break;
      case 3:
        if (is_expr(stream)) {
          current_state = 4;
          continue;
        }
        else return 1;
        break;
      case 4:
        return 1;
      break;
      case 5:
        if (is_expr(stream)) {
          current_state = 6;
          continue;
        }
        else
          return 0;
        break;
      case 6:
        if (token->class == RPAR)
          current_state = 7;
        else
          return 0;
        break;
      case 7:
        if (token->class == DO)
          current_state = 8;
        else
          return 0;
        break;
      case 8:
        if (is_stmt(stream)) {
          current_state = 10;
          continue;
        }
        else
          return 0;
        break;
      case 9:
        if (is_expr(stream)) {
          current_state = 11;
          continue;
        }
        else
          return 0;
        break;
      case 10:
        if (token->class == ELSE)
          current_state = 12;
        else if (token->class == END)
          current_state = 4;
        else
          return 0;
        break;
      case 11:
        if (token->class == RPAR)
          current_state = 13;
        else
          return 0;
        break;
      case 12:
        if (is_stmt(stream)) {
          current_state = 14;
          continue;
        }
        else
          return 0;
        break;
      case 13:
        if (token->class == DO)
          current_state = 12;
        else
          return 0;
        break;
      case 14:
        if (token->class == END)
          current_state = 4;
        else
          return 0;
      break;
    }
    
    token = next_token(stream);
  }
}

int is_assignment(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL) /* IS ROOT NODE */
      //return 0;
      return current_state == 4 ? 1 : 0; // IS FINAL STATE?


    printf("Assigment, state: <%d>, token: <%s>\n", current_state, token->value); 

    switch (current_state) {
      case 0:
        if (token->class == IDENTIFIER)
          current_state = 1;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 1:
        if (token->class == ASSIGN)
          current_state = 2;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 2:
        if (is_expr(stream)) {
          current_state = 3;
          continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
        }
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 3:
        if (token->class == SEMICOLON)
          current_state = 4;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 4: /* ACCEPT: FINAL STATE */
        return 1;
    }
    /* INVARIANT: always consume token */
    token = next_token(stream);
  }
}

int is_expr(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    if (token == NULL)
      return 0; /* ERROR: NOT ROOT NODE */

    printf("Expression, state: <%d>, token: <%s>\n", current_state, token->value);  

    switch (current_state) {
      case 0:
        if (is_factor(stream)) {
          current_state = 1;
          continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
        }
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 1:
        switch (token->class) {
          case MULT:
          case DIV:
          case ADD:
          case SUB:
            current_state = 0;
            break;
          case LT:
          case GT:
          case GE:
          case LE:
          case EQ:
          case NE:
            current_state = 2;
            break;
          default:
            return 1; /* ACCEPT: FINAL STATE */
        }
        break;
      case 2:
        if (is_factor(stream)) {
          current_state = 3;
          continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
        }
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 3:
        switch (token->class) {
          case MULT:
          case DIV:
          case ADD:
          case SUB:
            current_state = 2;
            break;
          default:
            return 1; /* ACCEPT: FINAL STATE */
        }
        break;
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
      
    printf("Factor, state: <%d>, token: <%s>\n", current_state, token->value);  

    switch (current_state) {
      case 0:
        switch (token->class) {
          case NUMBER:
            current_state = 1;
            break;
          case IDENTIFIER:
            current_state = 2;
            break;
          case OPERATION:
            current_state = 3;
            break;
          case LPAR:
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
        else if (token->class == LPAR)
          current_state = 7;
        else {
          return 1; /* ACCEPT: FINAL STATE */
        }
        break;
      case 3:
        if (token->class == LPAR)
          current_state = 7;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 4:
        if(is_expr(stream)) {
          current_state = 5;
          continue; /* SUBMACHINE CALL, DO NOT CONSUME TOKEN */
        }
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 5:
        if (token->class == RPAR)
          current_state = 1;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 6:
        if (token->class == NUMBER)
          current_state = 10;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 7:
        switch (token->class) {
          case NUMBER:
          case IDENTIFIER:
            current_state = 8;
            break;
          case RPAR:
            current_state = 1;
          default:
            return 0; /* ERROR: NOT FINAL STATE */
        }
        break;
      case 8:
        if (token->class == COMMA)
          current_state = 9;
        else if (token->class == RPAR)
          current_state = 1;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 9:
        if (token->class == NUMBER || token->class == IDENTIFIER)
          current_state = 8;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
      case 10:
        if (token->class == RBRA)
          current_state = 1;
        else
          return 0; /* ERROR: NOT FINAL STATE */
        break;
    }
    /* INVARIANT: always consume token */
    token = next_token(stream);
  }
}