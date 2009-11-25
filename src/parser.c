#include <stdio.h>

#include "parser.h"
#include "lexer.h"

static Token *token = NULL;

int parse(BufferedInputStream *source_code_stream) {
	return is_assignment(source_code_stream);
}

int is_assignment(BufferedInputStream *stream) {
	int current_state = 0;
	
	while (1) {
	  token = next_token(stream);
	
	  if (token == NULL)
      return 0;
    // printf("<%d>", current_state);

		switch (current_state) {
			case 0:
				if (token->class == IDENTIFIER)
					current_state = 1;
				else
					return 0;
				break;
			case 1:
				if (token->class == ASSIGN)
					current_state = 2;
				else
					return 0;
				break;
			case 2:
				if (is_expr(stream))
          current_state = 3;
				else
          return 0;
        break;
      case 3:
        if (token->class == SEMICOLON)
          return 1;
        break;
		}
	}
}

int is_expr(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    token = next_token(stream);
    
    if (token == NULL)
      return 0;

    // printf("state: %d \n", current_state);
    switch (current_state) {
      case 0:
        if (is_factor(stream))
          current_state = 1;
        else
          return 0;
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
          default:
            return 1;
        }
        break;
      case 2:
        if (is_factor(stream))
          current_state = 3;
        else
          return 0;
        break;
      case 3:
        switch (token->class) {
          case MULT:
          case DIV:
          case ADD:
          case SUB:
            current_state = 2;
          default:
            return 1;
        }
        break;
    }
  }
}

int is_factor(BufferedInputStream *stream) {
  return 1;
}