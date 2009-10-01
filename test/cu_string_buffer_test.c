#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_create_empty_string_buffer(CuTest*);
extern void test_append_string_to_buffer(CuTest*);
extern void test_append_more_strings_to_buffer(CuTest*);
extern void test_append_string_bigger_than_buffer(CuTest*);
extern void test_concat_string_buffers(CuTest*);
extern void test_clear_string_buffer(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_create_empty_string_buffer);
    SUITE_ADD_TEST(suite, test_append_string_to_buffer);
    SUITE_ADD_TEST(suite, test_append_more_strings_to_buffer);
    SUITE_ADD_TEST(suite, test_append_string_bigger_than_buffer);
    SUITE_ADD_TEST(suite, test_concat_string_buffers);
    SUITE_ADD_TEST(suite, test_clear_string_buffer);


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
