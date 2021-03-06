#ifndef __FA_H__
#define __FA_H__

#include <stdio.h>
#include <stdbool.h>
#include "bufstream.h"

#define NUMBER_CHARS 256

typedef int state_t;
typedef enum {
  ERROR,
  MOVEAPPEND,
  MOVENOAPPEND, 
  HALTAPPEND,
  HALTNOAPPEND,
  HALTREUSE
} action_t;



typedef struct finite_automata {
  char *subject;
  int nstates;
  state_t initial_state;
  state_t **transitions;
  action_t **actions;
} FiniteAutomata;

FiniteAutomata *fa_new(char *, int);
void fa_delete(FiniteAutomata *);

void fa_initial_state(FiniteAutomata *, state_t);
void fa_transition(FiniteAutomata *, state_t, const char *, state_t, action_t);
bool fa_run(FiniteAutomata *, char *, BufferedInputStream *);
#endif
