#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*Funcoes de erros*/
int overflow(){
  printf("Erro ocorreu, a pilha ja esta cheia\n");
  system ("PAUSE");
  exit(0);
}
int underflow(){
  printf("Erro ocorreu, a pilha esta vazia\n"); 
  system ("PAUSE");
  exit(0);
}

void stack_clean(Stack *s) {
  s->top = -1;
}

void stack_push(ItemType item, Stack *s)
{
  s->top++;
  if( s->top == MAX )
     overflow(); /* a pilha já está cheia*/
  else
     s->items[s->top] = item; 
}

ItemType stack_pop(Stack *s) {
  if (s->top == -1)
    underflow();
  else 
    return s->items[s->top--];
}

ItemType stack_lookup(Stack *s) {
  if (s->top == -1)
    underflow(); /* actually, in that scenario, we don't have a real underflow */
  else 
    return s->items[s->top];
}