#include "scanner.h"

#ifndef __LEXER_H__
#define __LEXER_H__

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

typedef struct lexer {
    Buffer *buffer;
    Token* (*read_token) (void);
} Lexer;

Lexer *lexer_new();
void lexer_delete(Lexer *lexer);

Token *word_new(int class, char *word);
Token *reserved_new(char *word);
Token *number_new(int num);
void token_delete(Token *token);

#endif