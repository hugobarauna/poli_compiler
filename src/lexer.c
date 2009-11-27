#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "hashtable.h"
#include "string_buffer.h"
#include "bufstream.h"
#include "lexer.h"

static Hashtable *ht = NULL;
static int is_reserved_tokens_loaded = 0;

static Token *retrieve_token(token_t type, char *str)
{
  Token *token = NULL;
  char *symbol = strdup(str);
  switch (type) {
  case T_SYMBOL:
    token = hashtable_get(ht, symbol);
    break;
  case T_NUMBER:
    token = token_new(NUMBER, symbol);
    break;
  case T_OPERATION:
    token = hashtable_get(ht, symbol);
    if (token == NULL) {
      token = token_new(OPERATION, symbol);
      hashtable_insert(ht, symbol, token);
    }
    break;
  case T_IDENTIFIER:
    token = hashtable_get(ht, symbol);
    if (token == NULL) {
      token = token_new(IDENTIFIER, symbol);
      hashtable_insert(ht, symbol, token);
    }
    break;
  case T_STRING:
    token = token_new(STRING, symbol);
    break;
  case T_GLOBAL:
    token = token_new(GLOBAL, symbol);
    break;
  }
  //printf("token { class : %d, value : %s }", token->class, token->value);
  return token;
}

static void load_reserved_tokens()
{
  ht = hashtable_new(1024);
  hashtable_insert(ht, "while", token_new(WHILE, "while"));
  hashtable_insert(ht, "if", token_new(IF, "if"));
  hashtable_insert(ht, "do", token_new(DO, "do"));
  hashtable_insert(ht, "else", token_new(ELSE, "else"));
  hashtable_insert(ht, "end", token_new(END, "end"));
  hashtable_insert(ht, "return", token_new(RETURN, "return"));
  hashtable_insert(ht, "int", token_new(INT, "int"));
  hashtable_insert(ht, "char", token_new(CHAR, "char"));
  hashtable_insert(ht, "bool", token_new(BOOL, "bool"));
  hashtable_insert(ht, "void", token_new(VOID, "void"));
  hashtable_insert(ht, "struct", token_new(STRUCT, "struct"));
  hashtable_insert(ht, "(", token_new(LPAR, "("));
  hashtable_insert(ht, ")", token_new(RPAR, ")"));
  hashtable_insert(ht, "{", token_new(LCUR, "{"));
  hashtable_insert(ht, "}", token_new(RCUR, "}"));
  hashtable_insert(ht, "[", token_new(LBRA, "["));
  hashtable_insert(ht, "]", token_new(RBRA, "]"));
  hashtable_insert(ht, "=", token_new(ASSIGN, "="));
  hashtable_insert(ht, "+=", token_new(ADD_ASSIGN, "+="));
  hashtable_insert(ht, "-=", token_new(SUB_ASSIGN, "-="));
  hashtable_insert(ht, "*=", token_new(MULT_ASSIGN, "*="));
  hashtable_insert(ht, "/=", token_new(DIV_ASSIGN, "/="));
  hashtable_insert(ht, "**=", token_new(PWR_ASSIGN, "**="));
  hashtable_insert(ht, "%=", token_new(MOD_ASSIGN, "%="));
  hashtable_insert(ht, "&=", token_new(AND_ASSIGN, "&="));
  hashtable_insert(ht, "|=", token_new(OR_ASSIGN, "|="));
  hashtable_insert(ht, "^=", token_new(NOT_ASSIGN, "^="));
  hashtable_insert(ht, "<<=", token_new(SHIFT_LEFT_ASSIGN, "<<="));
  hashtable_insert(ht, ">>=", token_new(SHIFT_RIGHT_ASSIGN, ">>="));
  hashtable_insert(ht, "&&=", token_new(LOGIC_AND_ASSIGN, "&&="));
  hashtable_insert(ht, "||=", token_new(LOGIC_OR_ASSIGN, "||="));
  hashtable_insert(ht, ":", token_new(COLON, ":"));
  hashtable_insert(ht, "::", token_new(DOUBLE_COLON, "::"));
  hashtable_insert(ht, "..", token_new(INCLUSIVE_RANGE, ".."));
  hashtable_insert(ht, "...", token_new(EXCLUSIVE_RANGE, "..."));
  hashtable_insert(ht, "|", token_new(OR, "|"));
  hashtable_insert(ht, "||", token_new(LOGIC_OR, "||"));
  hashtable_insert(ht, "^", token_new(XOR, "^"));
  hashtable_insert(ht, "&", token_new(AND, "&"));
  hashtable_insert(ht, "&&", token_new(LOGIC_AND, "&&"));
  hashtable_insert(ht, "<=>", token_new(COMPARE, "<=>"));
  hashtable_insert(ht, "==", token_new(EQ, "=="));
  hashtable_insert(ht, "===", token_new(SAME, "==="));
  hashtable_insert(ht, "=~", token_new(MATCH, "=~"));
  hashtable_insert(ht, "!~", token_new(NOT_MATCH, "!~"));
  hashtable_insert(ht, "<", token_new(LT, "<"));
  hashtable_insert(ht, ">", token_new(GT, ">"));
  hashtable_insert(ht, "<=", token_new(LE, "<="));
  hashtable_insert(ht, ">=", token_new(GE, ">="));
  hashtable_insert(ht, "!=", token_new(NE, "!="));
  hashtable_insert(ht, "++", token_new(INCREMENT, "++"));
  hashtable_insert(ht, "--", token_new(DECREMENT, "--"));
  hashtable_insert(ht, "+", token_new(ADD, "+"));
  hashtable_insert(ht, "-", token_new(SUB, "-"));
  hashtable_insert(ht, "*", token_new(MULT, "*"));
  hashtable_insert(ht, "/", token_new(DIV, "/"));
  hashtable_insert(ht, "%", token_new(MOD, "%"));
  hashtable_insert(ht, "**", token_new(PWR, "**"));
  hashtable_insert(ht, "<<", token_new(SHIFT_LEFT, "<<"));
  hashtable_insert(ht, ">>", token_new(SHIFT_RIGHT, ">>"));
  hashtable_insert(ht, "~", token_new(COMPLEMENT, "~"));
  hashtable_insert(ht, ",", token_new(COMMA, ","));
  hashtable_insert(ht, ";", token_new(SEMICOLON, ";"));
  hashtable_insert(ht, ".", token_new(DOT, "."));
  hashtable_insert(ht, "->", token_new(TO, "->"));
  hashtable_insert(ht, "=>", token_new(REFER, "=>"));
}

Token *token_new(class_t class, char *value) {
  Token *token = (Token *)malloc(sizeof(Token));
  if (token == NULL)
    fatal_error("couldn't create a token");

  token->class = class;
  token->value = value;

  return token;
}

void token_delete(Token *token) {
  if (token == NULL) return;
  free(token);
}

static void ignore_ws_and_comments(BufferedInputStream *stream)
{
  int current_state = 0;
  int ch;
  while (1) {
    ch = read(stream);
    switch (current_state) {
    case 0:
      if (ch == ' ' || ch == '\t' || ch == '\n')
        current_state = 1;
      else if (ch == '#')
        current_state = 2;
      else {
        unread(ch, stream);
        return;
      }
      break;
    case 1:
      if (ch == ' ' || ch == '\t' || ch == '\n')
        current_state = 1;
      else if (ch == '#')
        current_state = 2;
      else {
        unread(ch, stream);
        return;
      }
      break;
    case 2:
      if (isprint(ch) || ch == '\t')
        current_state = 2;
      else if (ch == '\n')
        current_state = 1;
      else {
        printf("ERROR");
        exit(1);
      }
      break;
    case 3:
      unread(ch, stream);
      return;
    }
  }
}

Token *next_token(BufferedInputStream *stream)
{
  StringBuffer *sb = NULL;
  int current_state = 0;
  token_t type = T_SYMBOL;
  int ch;

  if (!is_reserved_tokens_loaded) {
    load_reserved_tokens();
    is_reserved_tokens_loaded = 1;
  }

  if (finished(stream)) return NULL;

  ignore_ws_and_comments(stream);

  sb = sbuf_new(256);
  
  while (1) {
    ch = read(stream);
    switch (current_state) {
    case 0:
      if (ch == '+')
        current_state = 1;
      else if (ch == '-')
        current_state = 2;
      else if (ch == '*')
        current_state = 3;
      else if (ch == '/' || ch == '%' || ch == '^')
        current_state = 4;
      else if (ch == '&')
        current_state = 5;
      else if (ch == '|')
        current_state = 6;
      else if (ch == '<')
        current_state = 7;
      else if (ch == '>')
        current_state = 8;
      else if (ch == '=')
        current_state = 9;
      else if (ch == ',' || ch == ';' || ch == '~' 
         || ch == '[' || ch == ']' || ch == '(' 
         || ch == ')' || ch == '{' || ch == '}')
         current_state = 10;
      else if (ch == ':')
        current_state = 11;
      else if (ch == '.')
        current_state = 12;
      else if (ch == '!')
        current_state = 13;
      else if (ch == '$') {
        type = T_GLOBAL;
        current_state = 14;
      }
      else if (ch == '_' || isalpha(ch)) {
        type = T_IDENTIFIER;
        current_state = 15;
      }
      else if (isdigit(ch)) {
        type = T_NUMBER;
        current_state = 16;
      }
      else if (ch == '\"') {
        type = T_STRING;
        current_state = 17;
      }
      else if (ch == '\'') {
        type = T_STRING;
        current_state = 18;
      }
      else current_state = -1;
      break;
    case 1:
      if (ch == '=' || ch == '+')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 2:
      if (ch == '>' || ch == '=' || ch == '-')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 3:
      if (ch == '*')
        current_state = 4;
      else if (ch == '=')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 4:
      if (ch == '=')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 5:
      if (ch == '&')
        current_state = 4;
      else if (ch == '=')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 6:
      if (ch == '|')
        current_state = 4;
      else if (ch == '=')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 7:
      if (ch == '<')
        current_state = 4;
      else if (ch == '=')
        current_state = 21;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 8:
      if (ch == '>')
        current_state = 4;
      else if (ch == '=')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 9:
      if (ch == '=')
        current_state = 4;
      else if (ch == '>' || ch == '~')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 10:
      unread(ch, stream);
      return retrieve_token(type, to_str(sb));
      break;
    case 11:
      if (ch == ':')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 12:
      if (ch == '.')
        current_state = 22;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 13:
      if (ch == '=' || ch == '~')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 14:
      if (ch == '_' || isalpha(ch))
        current_state = 15;
      else current_state = -1;
      break;
    case 15:
      if (ch == '!' || ch == '?') {
        type = T_OPERATION;
        current_state = 10;
      }
      else if (ch == '_' || isalpha(ch) || isdigit(ch))
        current_state = 15;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 16:
      if (isdigit(ch))
        current_state = 16;
      else if (ch == '.')
        current_state = 19;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 17:
      if (ch == '\"')
        current_state = 10;
      else if (isprint(ch))
        current_state = 17;
      else current_state = -1;
      break;
    case 18:
      if (ch == '\'')
        current_state = 10;
      else if (isprint(ch))
        current_state = 18;
      else current_state = -1;
      break;
    case 19:
      if (isdigit(ch))
        current_state = 20;
      else current_state = -1;
      break;
    case 20:
      if (isdigit(ch))
        current_state = 20;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 21:
      if (ch == '>')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    case 22:
      if (ch == '.')
        current_state = 10;
      else {
        unread(ch, stream);
        current_state = 10;
        continue;
      }
      break;
    default:
      return NULL;
    }
    sbuf_append_char(sb, ch);
  }
}
