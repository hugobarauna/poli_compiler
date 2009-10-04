#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_create_empty_string_buffer(CuTest*);
extern void test_append_string_to_buffer(CuTest*);
extern void test_append_more_strings_to_buffer(CuTest*);
extern void test_append_string_bigger_than_buffer(CuTest*);
extern void test_concat_string_buffers(CuTest*);
extern void test_clear_string_buffer(CuTest*);
extern void test_create_hashtable(CuTest*);
extern void test_create_buffer(CuTest*);
extern void test_read_char_from_file(CuTest*);
extern void test_unread_char_to_buffer(CuTest*);
extern void test_start_with_unread_the_returned_char_should_not_be_a_lexeme(CuTest*);
extern void test_grow_buffer_when_token_is_bigger_than_buffer(CuTest*);
extern void test_retrieve_token_readed(CuTest*);
extern void test_extract_token_get_text_should_be_empty(CuTest*);
extern void test_when_finished_reading_file_EOF_is_returned(CuTest*);


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
    SUITE_ADD_TEST(suite, test_create_hashtable);
    SUITE_ADD_TEST(suite, test_create_buffer);
    SUITE_ADD_TEST(suite, test_read_char_from_file);
    SUITE_ADD_TEST(suite, test_unread_char_to_buffer);
    SUITE_ADD_TEST(suite, test_start_with_unread_the_returned_char_should_not_be_a_lexeme);
    SUITE_ADD_TEST(suite, test_grow_buffer_when_token_is_bigger_than_buffer);
    SUITE_ADD_TEST(suite, test_retrieve_token_readed);
    SUITE_ADD_TEST(suite, test_extract_token_get_text_should_be_empty);
    SUITE_ADD_TEST(suite, test_when_finished_reading_file_EOF_is_returned);


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
