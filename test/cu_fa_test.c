#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_create_a_finite_automata(CuTest*);
extern void test_define_a_initial_state(CuTest*);
extern void test_transition_definition(CuTest*);
extern void test_transition_range_defition(CuTest*);
extern void test_transition_with_escaped_char(CuTest*);
extern void test_transition_with_any_char(CuTest*);
extern void test_machine_execution(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_create_a_finite_automata);
    SUITE_ADD_TEST(suite, test_define_a_initial_state);
    SUITE_ADD_TEST(suite, test_transition_definition);
    SUITE_ADD_TEST(suite, test_transition_range_defition);
    SUITE_ADD_TEST(suite, test_transition_with_escaped_char);
    SUITE_ADD_TEST(suite, test_transition_with_any_char);
    SUITE_ADD_TEST(suite, test_machine_execution);


    CuSuiteRun(suite);
    CuSuiteSummary(suite, output);
    CuSuiteDetails(suite, output);
    printf("%s\n", output->buffer);
}

int main(void)
{
    RunAllTests();
    return 0;
}
