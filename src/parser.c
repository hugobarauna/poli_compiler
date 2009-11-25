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
	
	while(1) {
    // printf("<%d>", current_state);
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
				if (is_expr(stream))
					return 1;
				else
					return 0;
		}
	}
}

int is_expr(BufferedInputStream *stream) {
	return 1;
}
