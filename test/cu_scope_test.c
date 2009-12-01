#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_create_scope(CuTest*);
extern void test_reset_should_have_no_scope(CuTest*);
extern void test_insert_and_retrieve_entry_to_scope(CuTest*);
extern void test_do_not_find_entry_scope(CuTest*);
extern void test_search_on_parent_scope(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_create_scope);
    SUITE_ADD_TEST(suite, test_reset_should_have_no_scope);
    SUITE_ADD_TEST(suite, test_insert_and_retrieve_entry_to_scope);
    SUITE_ADD_TEST(suite, test_do_not_find_entry_scope);
    SUITE_ADD_TEST(suite, test_search_on_parent_scope);


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
