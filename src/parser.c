#include <stdio.h>
#include <stdbool.h>

#include "bufstream.h"
#include "lexer.h"
#include "error.h"

static Token *token = NULL;

int expr(BufferedInputStream *stream)
{
  int current_state = 0;
  while (1) {
    printf("<%d>", current_state);
    //printf("token = { class: %d, val: %s }\n", token->class, token->value);
    switch (current_state) {
    case 0:
      if (token->class == ID || token->class == NUMBER)
	      current_state = 1;
      else if (token->class == LPAR)
	      current_state = 2;
      else
	      return 0;
      token = next_token(stream);
      break;
    case 1:
      if (token->class == MULT || token->class == ADD)
	      current_state = 0;
      else
	      return 1;
      token = next_token(stream);
      break;
    case 2:
      printf("called expr() submachine\n");
      if (!expr(stream))
	      return 0;
      printf("return from expr()\n");
      current_state = 3;
      continue;
    case 3:
      printf("value: %s", token->value);
      if (token->class == RPAR) {
	      printf("entered here");
	      current_state = 1;
      }
      else
	      return 0;
      token = next_token(stream);
      break;
    }
  }
}

int stmt(BufferedInputStream *stream)
{
  int current_state = 0;
  while (1) {
    printf("<stmt:%d>", current_state);
    switch (current_state) {
    case 0:
      if (token->class == IF)
	      current_state = 1;
      else if (token->class == WHILE)
	      current_state = 3;
      else {
	      printf("stmt called expr()\n");
	      if (!expr(stream)) return 0;
	      printf("returned from expr() to stmt\n");
	      current_state = 2;
	      continue;
      }
      token = next_token(stream);
      break;
    case 1:
      printf("called expr()\n");
      if (!expr(stream)) return 0;
      printf("returned from expr()\n");
      current_state = 4;
      continue;
    case 2:
      return 1;
    case 3:
      printf("called expr()\n");
      if (!expr(stream)) return 0;
      printf("returned from expr()\n");
      current_state = 5;
      continue;
    case 4:
      if (token->class == DO)
	      current_state = 6;
      else return 0;
        token = next_token(stream);
      break;
    case 5:
      if (token->class == DO)
	      current_state = 7;
      else return 0;
        token = next_token(stream);
      break;
    case 6:
      printf("called stmt()\n");
      if (!stmt(stream))
	      return 0;
      printf("return from stmt()\n");
      current_state = 8;
      break;
    case 7:
      printf("called stmt()\n");
      if (!stmt(stream))
	      return 0;
      printf("return from stmt()\n");
      current_state = 9;
      break;
    case 8:
      if (token->class == ELSE)
	      current_state = 7;
      else if (token->class == END)
	      current_state = 2;
      else return 0;
      token = next_token(stream);
      break;
    case 9:
      if (token->class == END)
	      current_state = 2;
      else return 0;
      token = next_token(stream);
      break;
    }
  }
}

int compiler_prog(BufferedInputStream *stream)
{
  int current_state = 0;

   token = next_token(stream); /* LOOKAHEAD */
   while (1) {
     switch (current_state) {
     case 0:
       if (!stmt(stream)) return 0;
       current_state = 1;
       break;
     case 1:
       if (!stmt(stream)) return 0;
       current_state = 1;
       break;
     }
   }
}

int arg_list(BufferedInputStream *stream)
{
  return 1;
}

int or(BufferedInputStream *stream)
{
  int current_state = 0;
  while (1) {
    printf("<%d>\n", current_state);
    switch (current_state) {
    case 0:
      if (token->class == INCREMENT || token->class == DECREMENT)
	      current_state = 1;
      else if (token->class == ID || token->class == NUMBER)
	      current_state = 2;
      else
	      return 2;
      token = next_token(stream);
      break;
    case 1:
      if (token->class == ID || token->class == NUMBER)
	      current_state = 2;
      else return 0;
        token = next_token(stream);
      break;
    case 2:
      switch (token->class) {
      case INCREMENT:
      case DECREMENT:
	      current_state = 2;
	      token = next_token(stream);
	      break;
      case LBRA:
	      current_state = 3;
	      token = next_token(stream);
	      break;
      case LPAR:
	      current_state = 4;
	      token = next_token(stream);
	      break;
      case DOT:
      case TO:
	      current_state = 5;
	      token = next_token(stream);
	      break;
      case LOGIC_AND:
      case LOGIC_OR:
      case PWR:
      case OR:
      case XOR:
      case EQ:
      case NE:
      case SHIFT_RIGHT:
      case SHIFT_LEFT:
      case GT:
      case LT:
      case GE:
      case LE:
      case MULT:
      case DIV:
      case MOD:
      case ADD:
      case SUB:
	      current_state = 0;
	      token = next_token(stream);
	      break;
      default:
	      return 1;
      }
      break;
    case 3:
      printf("called expr()\n");
      if (expr(stream)) return 0;
      printf("returned from expr()\n");
      current_state = 6;
      continue;
      break;
    case 4:
      if (token->class == RPAR)
	      current_state = 2;
      else {
	      printf("called arg_list()\n");
	      arg_list(stream);
	      printf("returned from arg_list()\n");
	      current_state = 7;
	      continue;
      }
      token = next_token(stream);
      break;
    case 5:
      if (token->class == ID)
	      current_state = 2;
      else return 0;
      token = next_token(stream);
      break;
    case 6:
      if (token->class == RBRA)
	      current_state = 2;
      else return 0;
      token = next_token(stream);
      break;
    case 7:
      if (token->class == RPAR)
	      current_state = 2;
      else return 0;
      token = next_token(stream);
      break;
    }
  }
}
