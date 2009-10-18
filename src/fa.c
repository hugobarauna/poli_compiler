#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

#include "fa.h"
#include "bufstream.h"
#include "xstring.h"
#include "alloc.h"
#include "error.h"

/*
 * Creates a FiniteAutomata instance with a subject text (purpouse) and
 * a number of states.
 */
FiniteAutomata *fa_new(char *subject, int number_of_states)
{
  if (number_of_states <= 0)
    fatal_error("Must have at least one state");

  FiniteAutomata *machine = (FiniteAutomata *)malloc(sizeof(FiniteAutomata));
  if (machine == NULL)
    fatal_error("Couldn't create FiniteAutomata");
  
  machine->subject = subject;
  machine->nstates = number_of_states;
  machine->transitions = (state_t **)talloc(machine->nstates, NUMBER_CHARS, sizeof(state_t));
  if (machine->transitions == NULL)
    fatal_error("Couldn't create the transition table");

  machine->actions = (action_t **)talloc(machine->nstates, NUMBER_CHARS, sizeof(action_t));
  if (machine->actions == NULL)
    fatal_error("Couldn't create the action table");

  machine->initial_state = -1;

  return machine;
}

/*
 * Completelly removes a FiniteAutomata.
 */
void fa_delete(FiniteAutomata *machine)
{
  if (machine == NULL) return;
  if (machine->transitions != NULL) {
    free(machine->transitions);
    machine->transitions = NULL;
  }
  if (machine->actions != NULL) {
    free(machine->actions);
    machine->actions = NULL;
  }
  free(machine);
}

/*
 * Defines the initial state for this machine.
 */
void fa_initial_state(FiniteAutomata *machine, state_t state)
{
  machine->initial_state = state;
}

/*
 * Defines a transition and action given a state and input and it next state.
 *
 * The actions can be:
 * - ERROR: the current token is rollback;
 * - MOVEAPPEND: move to next state, consumes the character and add the char 
 *               to token
 * - MOVENOAPPEND: move to next state, consumes the character
 * - HALTAPPEND: terminates the machine execution, consumes the character and
 *               add character to token
 * - HALTNOAPPEND: terminates the machine, consumes the character
 * - HALTREUSE: terminates the machine and put back the character
 *
 * Transitions accepts expression to define the inputs accepted from a state.
 * This simplifies the description which accepts alphacharacters (a, b, c...)
 * by using a simplified regular expression notation.
 *
 * Accepted notations:
 *  X-Y : add a range of characteres, from X to Y
 *  \x  : escapes the character
 *  *   : set to all states with no action defined (action=ERROR)
 *  x   : adds a transition for a single char
 */
void fa_transition(FiniteAutomata *machine, state_t from, const char *inputs, state_t to, action_t action)
{
  int i;
  const char *ptr = NULL;
  char start, end;
  if (inputs == NULL) return;

  ptr = inputs;
  do {
    start = end = *ptr++;
    if (start == '*') {
      for (i = 0; i <= 255; ++i) {
	if (machine->actions[from][i] != ERROR) continue;
	machine->transitions[from][i] = to;
	machine->actions[from][i] = action;
      }
      return;
    }
    else if (start == '\\') {
      i = *ptr;
      machine->transitions[from][i] = to;
      machine->actions[from][i] = action;
      return;
    }
    else if (*ptr == '-') {
      end = *(++ptr);
      ptr++;
    }
    for (i = start; i <= end; ++i) {
      machine->transitions[from][i] = to;
      machine->actions[from][i] = action;
    }
  } while (*ptr != '\0');
}

/*
 * Runs the machine and returns true or false if the machine accepts
 * the input.
 *
 * The token read is saved on token, so it expects that the size is
 * bigger enough to put the token. (this is not safe, and must be
 * refactored)
 */
bool fa_run(FiniteAutomata *m, char *token, BufferedInputStream *in)
{
  state_t state = m->initial_state;
  int c, j = 0;;
  mark(in);
  while (true) {
    c = read(in);
    //printf("%c", c);
    switch (m->actions[state][c]) {
    case ERROR:
      //printf(" -> ERROR\n");
      token[j++] = c;
      token[j] = '\0';
      reverse1(token);
      while (j > 0) {/* pushback token consumed */
        unread(token[--j], in);
	    token[j] = '\0'; /* cleaning token */
      }
      unmark(in);
      return false;
    case MOVEAPPEND:
      //printf(" -> MOVEAPPEND\n");
      state = m->transitions[state][c];
      token[j++] = c;
      break;
    case MOVENOAPPEND:
      //printf(" -> MOVENOAPPEND\n");
      state = m->transitions[state][c];
      break;
    case HALTAPPEND:
      //printf(" -> HALTAPPEND\n");
      token[j++] = c;
      token[j] = '\0';
      unmark(in);
      return true;
    case HALTNOAPPEND:
      //printf(" -> HALTNOAPPEND\n");
      token[j] = '\0';
      unmark(in);
      return true;
    case HALTREUSE:
      //printf(" -> HALTREUSE\n");
      unread(c, in);
      token[j] = '\0';
      unmark(in);
      return true;
    }
  }
}
