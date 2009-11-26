#include <stdio.h>

#include "parser.h"
#include "lexer.h"

static Token *token = NULL;

int parse(BufferedInputStream *source_code_stream) {
	token = next_token(source_code_stream);
	return is_assignment(source_code_stream);
}

int is_assignment(BufferedInputStream *stream) {
	int current_state = 0;
	
	while (1) {
    // token = next_token(stream);
    printf("Assigment, state: <%d>, token: <%s>\n", current_state, token->value);	
	  if (token == NULL)
      return 0;

		switch (current_state) {
			case 0:
				if (token->class == IDENTIFIER)
					current_state = 1;
				else
					return 0;
				token = next_token(stream);
				break;
			case 1:
				if (token->class == ASSIGN)
					current_state = 2;
				else
          return 0;
        token = next_token(stream);
				break;
			case 2:
				if (is_expr(stream)) {
          current_state = 3;
          continue;
        }
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
    printf("Expression, state: <%d>, token: <%s>\n", current_state, token->value);	

    if (token == NULL)
      return 0;

    switch (current_state) {
      case 0:
        if (is_factor(stream)) {
          current_state = 1;
          continue;
        }
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
    	  token = next_token(stream);
        break;
      case 2:
        if (is_factor(stream)) {
          current_state = 3;
          continue;
        }
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
            token = next_token(stream);
            return 1;
        }
        token = next_token(stream);
        break;
    }
  }
}

int is_factor(BufferedInputStream *stream) {
  int current_state = 0;
  
  while (1) {
    printf("Factor, state: <%d>, token: <%s>\n", current_state, token->value);	
    
    if (token == NULL)
      return 0;
      
    switch (current_state) {
      case 0:
        switch (token->class) {
          case NUMBER:
            // current_state = 1;
            token = next_token(stream);
            return 1;
          case IDENTIFIER:
            current_state = 2;
            break;
          case OPERATION:
            current_state = 3;
            break;
          case LPAR:
            current_state = 4;
            break;
          default:
            return 0;
        }
        break;
      case 2:
        if (token->class == LBRA)
          current_state = 6;
        else if (token->class == LPAR)
          current_state = 7;
        else {
          token = next_token(stream); 
          return 1;
        }
        break;
      case 3:
        if (token->class == LPAR)
          current_state = 7;
        else
          return 0;
        break;
      case 4:
        if(is_expr(stream)) {
          current_state = 5;
          continue;
        }
        else
          return 0;
        break;
      case 5:
        if (token->class == RPAR) {
          // current_state = 1;
          token = next_token(stream);
          return 1;
        }
        else
          return 0;
        break;
      case 6:
        if (token->class == NUMBER)
          current_state = 10;
        else
          return 0;
        break;
      case 7:
        switch (token->class) {
          case NUMBER:
          case IDENTIFIER:
            current_state = 8;
            break;
          case RPAR:
            token = next_token(stream);
            return 1;
            // current_state = 1;
          default:
            return 0;
        }
        break;
      case 8:
        if (token->class == COMMA)
          current_state = 9;
        else if (token->class == RPAR) {
          // current_state = 1;
          token = next_token(stream);
          return 1;
        }
        else
          return 0;
        break;
      case 9:
        if (token->class == NUMBER || token->class == IDENTIFIER)
          current_state = 8;
        else
          return 0;
        break;
      case 10:
        if (token->class == RBRA) {
          // current_state = 1;
          token = next_token(stream);
          return 1;
        }
        else
          return 0;
        break;
    }

	  token = next_token(stream);
  }
}