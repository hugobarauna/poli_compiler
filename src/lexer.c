#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexer.h"
#include "fa.h"
#include "bufstream.h"
#include "error.h"

static FiniteAutomata *ws_machine, *delim_machine, *number_machine, *name_machine, *oper_machine;
static bool initialized = false;

static void white_space_machine()
{
  ws_machine = fa_new("white space", 2);
  fa_initial_state(ws_machine, 0);
  fa_transition(ws_machine, 0, " \n\t;", 0, MOVENOAPPEND);
  fa_transition(ws_machine, 0, "*", 1, HALTREUSE);
}

static void delimiter_machine()
{
  delim_machine = fa_new("delimiters", 2);
  fa_initial_state(delim_machine, 0);
  fa_transition(delim_machine, 0, "()[]{}", 1, HALTAPPEND);
}

static void numeric_machine()
{
  number_machine = fa_new("number", 10);
  fa_initial_state(number_machine, 0);
  fa_transition(number_machine, 0, "0-9", 1, MOVEAPPEND);
  fa_transition(number_machine, 1, "0-9", 1, MOVEAPPEND);
  fa_transition(number_machine, 1, ".", 1, MOVEAPPEND);
  fa_transition(number_machine, 1, "E", 4, MOVEAPPEND);
  fa_transition(number_machine, 1, "*", 8, HALTREUSE);
  fa_transition(number_machine, 2, "0-9", 3, MOVEAPPEND);
  fa_transition(number_machine, 3, "0-9", 3, MOVEAPPEND);
  fa_transition(number_machine, 3, "E", 4, MOVEAPPEND);
  fa_transition(number_machine, 3, "*", 9, HALTREUSE);
  fa_transition(number_machine, 4, "0-9", 6, MOVEAPPEND);
  fa_transition(number_machine, 4, "+\\-", 5, MOVEAPPEND);
  fa_transition(number_machine, 5, "0-9", 6, MOVEAPPEND);
  fa_transition(number_machine, 6, "0-9", 6, MOVEAPPEND);
  fa_transition(number_machine, 6, "*", 7, HALTREUSE);
}

static void identifier_machine()
{
  name_machine = fa_new("reserved words", 3);
  fa_initial_state(name_machine, 0);
  fa_transition(name_machine, 0, "a-zA-Z_", 1, MOVEAPPEND);
  fa_transition(name_machine, 1, "a-zA-Z_0-9", 1, MOVEAPPEND);
  fa_transition(name_machine, 1, "?", 2, HALTAPPEND);
  fa_transition(name_machine, 1, "*", 2, HALTREUSE);
}

static void operator_machine()
{
  oper_machine = fa_new("operators", 9);
  fa_initial_state(oper_machine, 0);
  fa_transition(oper_machine, 0, "<", 1, MOVEAPPEND);
  fa_transition(oper_machine, 0, "\\*", 4, MOVEAPPEND);
  fa_transition(oper_machine, 0, "|", 6, MOVEAPPEND);
  fa_transition(oper_machine, 0, "&", 7, MOVEAPPEND);
  fa_transition(oper_machine, 0, ">/%!+=\\-", 5, MOVEAPPEND);
  fa_transition(oper_machine, 1, "=", 2, MOVEAPPEND);
  fa_transition(oper_machine, 1, "*", 8, HALTREUSE);
  fa_transition(oper_machine, 2, ">", 3, HALTAPPEND);
  fa_transition(oper_machine, 2, "*", 8, HALTREUSE);
  fa_transition(oper_machine, 4, "\\*", 5, MOVEAPPEND);
  fa_transition(oper_machine, 4, "=", 3, HALTAPPEND);
  fa_transition(oper_machine, 4, "*", 8, HALTREUSE);
  fa_transition(oper_machine, 5, "=", 3, HALTAPPEND);
  fa_transition(oper_machine, 5, "*", 8, HALTREUSE);
  fa_transition(oper_machine, 6, "|", 3, HALTAPPEND);
  fa_transition(oper_machine, 7, "&", 3, HALTAPPEND);
}

void init_lexer()
{
  if (initialized == true) return;

  white_space_machine();
  delimiter_machine();
  numeric_machine();
  identifier_machine();
  operator_machine();
  initialized = true;
}

void destroy_lexer()
{
  if (initialized == false) return;
  fa_delete(ws_machine);
  fa_delete(delim_machine);
  fa_delete(number_machine);
  fa_delete(name_machine);
  fa_delete(oper_machine);
  initialized = false;
}

Token *read_token(BufferedInputStream *in) {
    static char token[1000];
    fa_run(ws_machine, token, in);

    if (finished(in))
        return NULL;

    if (fa_run(oper_machine, token, in)) {
        return reserved_new(token);
    }
    else if (fa_run(delim_machine, token, in)) {
        return reserved_new(token);
    }
    else if (fa_run(number_machine, token, in)) {
        return number_new(atoi(token));
    }
    else if (fa_run(name_machine, token, in)) {
        return word_new(ID, token);
    }
    else {
        fatal_error("unknown token\n");
    }
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