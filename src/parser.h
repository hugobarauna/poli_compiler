#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include "bufstream.h"

/* VARIAVEIS, FUNCOES, VETOR */

int parse(BufferedInputStream *source_code);
int is_decl(BufferedInputStream *source_code);
int is_program(BufferedInputStream *source_code);
int is_stmt(BufferedInputStream *source_code);
int is_assignment(BufferedInputStream *stream);
int is_expr(BufferedInputStream *stream);
int is_factor(BufferedInputStream *stream);

#endif