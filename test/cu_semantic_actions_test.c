#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_sym_table_insert_and_get(CuTest*);
extern void test_sym_table_get_miss(CuTest*);
extern void test_generate_label(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_sym_table_insert_and_get);
    SUITE_ADD_TEST(suite, test_sym_table_get_miss);
    SUITE_ADD_TEST(suite, test_generate_label);


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
