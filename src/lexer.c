#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "scanner.h"

#define READ(ch) ((c = read()) == (ch))

static void fatal_error(char *msg) {
    printf("%s\n", msg);
    exit(1);
}

static FILE *src_file;
static Buffer *current_buffer;
static int line = 0;
static int c = ' ';

static Token *read_token() {
    for ( ; ; c = read()) {
        if (c == ' ' || c == '\t') {
            extract_text();
            continue;
        }
        else if (c == '\n') {
            extract_text();
            line = line + 1;
        }
        else break;
    }
    switch (c)
    {
        case '&':
        if (READ('&')) {
            c = ' ';
            return reserved_new(extract_text());
        }
        else {
            unread(c);
            return word_new(ID, extract_text());
        }
        case '|':
        if (READ('|')) {
            c = ' ';
            return reserved_new(extract_text());
        }
        else {
            unread(c);
            return word_new(ID, extract_text());
        }
        case '=':
        if (READ('=')) {
            c = ' ';
            return reserved_new(extract_text());
        }
        else {
            unread(c);
            return word_new(ID, extract_text());
        }
        case '!':
        if (READ('=')) {
            c = ' ';
            return reserved_new(extract_text());
        }
        else {
            unread(c);
            return word_new(ID, extract_text());
        }
        case '<':
        if (READ('=')) {
            c = ' ';
            return reserved_new(extract_text());
        }
        else {
            unread(c);
            return word_new(ID, extract_text());
        }
        case '>':
        if (READ('=')) {
            c = ' ';
            return reserved_new(extract_text());
        }
        else {
            unread(c);
            return word_new(ID, extract_text());
        }
    }
    if (isdigit(c)) {
        int v = 0;
        do {
            v = 10*v + (c - '0');
            c = read();
        } while (isdigit(c));
        unread(c);  extract_text();
        return number_new(v);
    }
    if (isalpha(c)) {
        do {
            c = read();
        } while(isalnum(c));
        unread(c);
        return word_new(ID, extract_text());
    }
    if (c == EOF) return NULL;
    return word_new(UNKNOWN, extract_text());
}

Lexer *lexer_new(const char *filepath) {
    src_file = fopen(filepath, "r");
    if (src_file == NULL)
        fatal_error("Couldn't open file.");
    
    Lexer *lexer = (Lexer *) malloc(sizeof(Lexer));
    if (lexer == NULL)
        fatal_error("couldn't dynamic alloc a Lexer instance.");
    
    lexer->read_token = read_token;
    lexer->buffer = new_buffer(src_file, 4096);
    current_buffer = lexer->buffer;
    
    return lexer;
}

void lexer_delete(Lexer *lexer)
{
    if (lexer == NULL) return;
    if (lexer->buffer != NULL) {
        delete_buffer(lexer->buffer);
        lexer->buffer = NULL;
    }
    free(lexer);
}



Token *word_new(int class, char *word)
{
    Token *token = (Token *) malloc(sizeof(Token));
    if (token == NULL)
        fatal_error("couldn't dynamic alloc token.");
    
    token->class = class;
    token->value.word = word;
    
    return token;
}

Token *reserved_new(char *word)
{
    return word_new(RESERVED, word);
}

Token *number_new(int num) {
    Token *token = (Token *) malloc(sizeof(Token));
    if (token == NULL)
        fatal_error("couldn't dynamic alloc token.");
    
    token->class = NUMBER;
    token->value.number = num;
    
    return token;
}

void token_delete(Token *token)
{
    if (token == NULL) return;
    free(token);
}