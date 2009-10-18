#include <stdio.h>
#include "CuTest.h"
#include "../src/fa.h"
#include "../src/bufstream.h"

#define NUMBER_STATES 3

static FILE *fp;
static BufferedInputStream *in;
static FiniteAutomata *m;

static void setup()
{
    fp = fopen("scanner.in", "r");
    in = buffered_input_stream_new(fp, DEFAULT_BUFFER_SIZE);
    m = fa_new("test", NUMBER_STATES);
}

static void teardown()
{
    fa_delete(m);
    buffered_input_stream_delete(in);
}

void test_create_a_finite_automata(CuTest *tc)
{
    setup();
    CuAssertStrEquals(tc, "test", m->subject);
    CuAssertIntEquals(tc, -1, m->initial_state);
    CuAssertIntEquals(tc, NUMBER_STATES, m->nstates);
    teardown();
}

void test_define_a_initial_state(CuTest *tc)
{
    setup();
    fa_initial_state(m, 0);
    CuAssertIntEquals(tc, 0, m->initial_state);
    teardown();
}

void test_transition_definition(CuTest *tc)
{
    setup();
    fa_transition(m, 0, "ab", 1, HALTNOAPPEND);
    CuAssertIntEquals(tc, 1, m->transitions[0]['a']);
    CuAssertIntEquals(tc, 1, m->transitions[0]['b']);
    CuAssertIntEquals(tc, HALTNOAPPEND, m->actions[0]['a']);
    teardown();
}

void test_transition_range_defition(CuTest *tc)
{
    int i;
    setup();
    fa_transition(m, 0, "a-z", 1, HALTNOAPPEND);
    
    for (i = 'a'; i <= 'z'; ++i) {
        CuAssertIntEquals(tc, 1, m->transitions[0][i]);
        CuAssertIntEquals(tc, HALTNOAPPEND, m->actions[0][i]);
    }

    CuAssertIntEquals(tc, ERROR, m->actions[0]['A']);

    teardown();
}

void test_transition_with_escaped_char(CuTest *tc)
{
    int i;
    setup();
    fa_transition(m, 0, "\\-", 1, HALTNOAPPEND);

    CuAssertIntEquals(tc, 1, m->transitions[0]['-']);
    CuAssertIntEquals(tc, HALTNOAPPEND, m->actions[0]['-']);

    teardown();
}

void test_transition_with_any_char(CuTest *tc)
{
    int i;
    setup();
    
    for (i = 0; i < 256; ++i)
        CuAssertIntEquals(tc, ERROR, m->actions[0][i]);

    fa_transition(m, 0, "*", 1, HALTNOAPPEND);

    for (i = 0; i < 256; ++i)
        CuAssertIntEquals(tc, HALTNOAPPEND, m->actions[0][i]);

    teardown();
}

void test_machine_execution(CuTest *tc)
{
    char token[10];
    setup();
    fa_initial_state(m, 0);
    fa_transition(m, 0, "a-z", 0, MOVEAPPEND);
    fa_transition(m, 0, "*", 1, HALTNOAPPEND);
    
    CuAssertIntEquals(tc, 0, m->transitions[0]['h']);
    
    fa_run(m, token, in);
    
    CuAssertStrEquals(tc, "hello", token);
    teardown();
}