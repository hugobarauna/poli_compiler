#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include "bufstream.h"
#include "lexer.h"
#include "stack.h"
#include "semantic_actions.h"

/* VARIAVEIS, FUNCOES, VETOR */

extern int variables_counter;

int parse(BufferedInputStream *source_code);
int is_decl(BufferedInputStream *source_code);
int is_program(BufferedInputStream *source_code);
int is_stmt(BufferedInputStream *source_code);
int is_assignment(BufferedInputStream *stream);
int is_expr(BufferedInputStream *stream);
int is_factor(BufferedInputStream *stream);

#endif