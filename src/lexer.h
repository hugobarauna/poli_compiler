#ifndef __LEXER_H__
#define __LEXER_H__

#include "bufstream.h"

typedef enum e_tag {
  IDENTIFIER,
  OPERATION,
  STRING,
  NUMBER,
  GLOBAL,
  WHILE,
  IF,
  DO,
  ELSE,
  END,
  RETURN,
  INT,
  CHAR,
  BOOL,
  VOID,
  STRUCT,
  SCAN,
  PRINT,
  LPAR,
  RPAR,
  LCUR,
  RCUR,
  LBRA,
  RBRA,
  ASSIGN,
  ADD_ASSIGN,
  SUB_ASSIGN,
  MULT_ASSIGN,
  DIV_ASSIGN,
  PWR_ASSIGN,
  MOD_ASSIGN,
  AND_ASSIGN,
  OR_ASSIGN,
  NOT_ASSIGN,
  SHIFT_LEFT_ASSIGN,
  SHIFT_RIGHT_ASSIGN,
  LOGIC_AND_ASSIGN,
  LOGIC_OR_ASSIGN,
  COLON,
  DOUBLE_COLON,
  INCLUSIVE_RANGE,
  EXCLUSIVE_RANGE,
  OR,
  LOGIC_OR,
  XOR,
  AND,
  LOGIC_AND,
  COMPARE,
  EQ,
  SAME,
  MATCH,
  NOT_MATCH,
  LT,
  GT,
  LE,
  GE,
  NE,
  ADD,
  SUB,
  INCREMENT,
  DECREMENT,
  MULT,
  DIV,
  MOD,
  PWR,
  SHIFT_LEFT,
  SHIFT_RIGHT,
  COMPLEMENT,
  COMMA,
  SEMICOLON,
  DOT,
  TO,
  REFER
} class_t;

typedef enum e_token_t {
  T_SYMBOL,
  T_IDENTIFIER,
  T_OPERATION,
  T_NUMBER,
  T_STRING,
  T_GLOBAL
} token_t;

typedef struct _token {
  class_t class;
  char *value;
} Token;

Token *token_new(class_t, char *);
void token_delete(Token *token);
Token *next_token(BufferedInputStream *);

#endif