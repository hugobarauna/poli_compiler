#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*Funcoes de erros*/
int overflow(const char *name){
  printf("Erro ocorreu, a pilha '%s' ja esta cheia\n", name);
  system ("PAUSE");
  exit(0);
}
int underflow(const char *name){
  printf("Erro ocorreu, a pilha '%s' esta vazia\n", name);
  system ("PAUSE");
  exit(0);
}

void stack_clean(Stack *s) {
  s->top = -1;
}

void stack_push(ItemType *item, Stack *s)
{
  s->top++;
  if( s->top == MAX )
     overflow(s->name); /* a pilha já está cheia*/
  else
     s->items[s->top] = item; 
}

ItemType *stack_pop(Stack *s) {
  if (s->top == -1)
    underflow(s->name);
//  printf("POPPED item from %s\n", s->name);
  return s->items[s->top--];
}

ItemType *stack_lookup(Stack *s) {
  if (s->top == -1)
    return NULL;
    
  return s->items[s->top];
}

int stack_empty(Stack *s) {
  return s->top == -1;
}