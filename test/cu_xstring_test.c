#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_convert_an_string_to_integer(CuTest*);
extern void test_convert_an_integer_to_string(CuTest*);
extern void test_reverse_a_string(CuTest*);
extern void test_reverse1_to_reverse_the_string_itself(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_convert_an_string_to_integer);
    SUITE_ADD_TEST(suite, test_convert_an_integer_to_string);
    SUITE_ADD_TEST(suite, test_reverse_a_string);
    SUITE_ADD_TEST(suite, test_reverse1_to_reverse_the_string_itself);


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
