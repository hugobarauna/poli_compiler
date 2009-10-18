#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_create_buffered_input_stream(CuTest*);
extern void test_read_a_char_from_file(CuTest*);
extern void test_unread_a_char_from_file(CuTest*);
extern void test_unread_with_empty_buffer_should_be_added_at_end_of_buffer(CuTest*);
extern void test_when_end_of_buffer_is_reach_the_buffer_do_not_grow(CuTest*);
extern void test_grow_buffer_when_marked(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_create_buffered_input_stream);
    SUITE_ADD_TEST(suite, test_read_a_char_from_file);
    SUITE_ADD_TEST(suite, test_unread_a_char_from_file);
    SUITE_ADD_TEST(suite, test_unread_with_empty_buffer_should_be_added_at_end_of_buffer);
    SUITE_ADD_TEST(suite, test_when_end_of_buffer_is_reach_the_buffer_do_not_grow);
    SUITE_ADD_TEST(suite, test_grow_buffer_when_marked);


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
