#ifndef __LEXER_H__
#define __LEXER_H__

#include "bufstream.h"

#define ID 0
#define NUMBER 1
#define RESERVED 2
#define UNKNOWN 3

typedef struct token {
    int class;
    union {
        int number;
        char *word;
    } value;
} Token;

void init_lexer();
void destroy_lexer();
Token *read_token(BufferedInputStream *);

Token *word_new(int class, char *word);
Token *reserved_new(char *word);
Token *number_new(int num);
void token_delete(Token *token);

#endif