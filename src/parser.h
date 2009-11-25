#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdlib.h>
#include "bufstream.h"

int parse(BufferedInputStream *source_code);
int is_assignment(BufferedInputStream *stream);
int is_expr(BufferedInputStream *stream);

#endif