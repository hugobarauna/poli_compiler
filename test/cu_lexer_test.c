#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_read_all_tokens(CuTest*);
extern void test_number_token(CuTest*);
extern void test_dump_token_word(CuTest*);
extern void test_should_return_NULL_if_there_is_no_more_token(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_read_all_tokens);
    SUITE_ADD_TEST(suite, test_number_token);
    SUITE_ADD_TEST(suite, test_dump_token_word);
    SUITE_ADD_TEST(suite, test_should_return_NULL_if_there_is_no_more_token);


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
