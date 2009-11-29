#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include "bufstream.h"
#include "lexer.h"
#include "stack.h"
#include "semantic_actions.h"
#include "error.h"

/* VARIAVEIS, FUNCOES, VETOR */

int parse(BufferedInputStream *source_code);
int is_prog(BufferedInputStream *source_code);
int is_external_decl(BufferedInputStream *source_code);
int is_decl(BufferedInputStream *source_code);
int is_compound_stmt(BufferedInputStream *source_code);
int is_stmt(BufferedInputStream *source_code);
int is_assignment(BufferedInputStream *source_code);
int is_expr(BufferedInputStream *source_code);
int is_factor(BufferedInputStream *source_code);

#endif