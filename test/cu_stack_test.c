#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_stack_clean(CuTest*);
extern void test_stack_push(CuTest*);
extern void test_stack_pop(CuTest*);
extern void test_stack_lookup(CuTest*);
extern void test_new_stack_empty(CuTest*);
extern void test_stack_empty(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_stack_clean);
    SUITE_ADD_TEST(suite, test_stack_push);
    SUITE_ADD_TEST(suite, test_stack_pop);
    SUITE_ADD_TEST(suite, test_stack_lookup);
    SUITE_ADD_TEST(suite, test_new_stack_empty);
    SUITE_ADD_TEST(suite, test_stack_empty);


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
