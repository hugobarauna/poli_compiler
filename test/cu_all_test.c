#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_table_allocation(CuTest*);
extern void test_create_buffered_input_stream(CuTest*);
extern void test_read_a_char_from_file(CuTest*);
extern void test_unread_a_char_from_file(CuTest*);
extern void test_unread_with_empty_buffer_should_be_added_at_end_of_buffer(CuTest*);
extern void test_when_end_of_buffer_is_reach_the_buffer_do_not_grow(CuTest*);
extern void test_grow_buffer_when_marked(CuTest*);
extern void test_create_a_finite_automata(CuTest*);
extern void test_define_a_initial_state(CuTest*);
extern void test_transition_definition(CuTest*);
extern void test_transition_range_defition(CuTest*);
extern void test_transition_with_escaped_char(CuTest*);
extern void test_transition_with_any_char(CuTest*);
extern void test_machine_execution(CuTest*);
extern void test_create_hashtable(CuTest*);
extern void test_insert_an_element(CuTest*);
extern void test_get_an_element(CuTest*);
extern void test_get_an_element_in_an_empty_hashtable(CuTest*);
extern void test_get_an_element_with_colision(CuTest*);
extern void test_get_an_null_element_with_colision(CuTest*);
extern void test_internal_key_should_be_inside_range_of_table_indexes(CuTest*);
extern void test_read_all_tokens(CuTest*);
extern void test_create_empty_string_buffer(CuTest*);
extern void test_append_string_to_buffer(CuTest*);
extern void test_append_more_strings_to_buffer(CuTest*);
extern void test_append_string_bigger_than_buffer(CuTest*);
extern void test_concat_string_buffers(CuTest*);
extern void test_clear_string_buffer(CuTest*);
extern void test_convert_an_string_to_integer(CuTest*);
extern void test_convert_an_integer_to_string(CuTest*);
extern void test_reverse_a_string(CuTest*);
extern void test_reverse1_to_reverse_the_string_itself(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_table_allocation);
    SUITE_ADD_TEST(suite, test_create_buffered_input_stream);
    SUITE_ADD_TEST(suite, test_read_a_char_from_file);
    SUITE_ADD_TEST(suite, test_unread_a_char_from_file);
    SUITE_ADD_TEST(suite, test_unread_with_empty_buffer_should_be_added_at_end_of_buffer);
    SUITE_ADD_TEST(suite, test_when_end_of_buffer_is_reach_the_buffer_do_not_grow);
    SUITE_ADD_TEST(suite, test_grow_buffer_when_marked);
    SUITE_ADD_TEST(suite, test_create_a_finite_automata);
    SUITE_ADD_TEST(suite, test_define_a_initial_state);
    SUITE_ADD_TEST(suite, test_transition_definition);
    SUITE_ADD_TEST(suite, test_transition_range_defition);
    SUITE_ADD_TEST(suite, test_transition_with_escaped_char);
    SUITE_ADD_TEST(suite, test_transition_with_any_char);
    SUITE_ADD_TEST(suite, test_machine_execution);
    SUITE_ADD_TEST(suite, test_create_hashtable);
    SUITE_ADD_TEST(suite, test_insert_an_element);
    SUITE_ADD_TEST(suite, test_get_an_element);
    SUITE_ADD_TEST(suite, test_get_an_element_in_an_empty_hashtable);
    SUITE_ADD_TEST(suite, test_get_an_element_with_colision);
    SUITE_ADD_TEST(suite, test_get_an_null_element_with_colision);
    SUITE_ADD_TEST(suite, test_internal_key_should_be_inside_range_of_table_indexes);
    SUITE_ADD_TEST(suite, test_read_all_tokens);
    SUITE_ADD_TEST(suite, test_create_empty_string_buffer);
    SUITE_ADD_TEST(suite, test_append_string_to_buffer);
    SUITE_ADD_TEST(suite, test_append_more_strings_to_buffer);
    SUITE_ADD_TEST(suite, test_append_string_bigger_than_buffer);
    SUITE_ADD_TEST(suite, test_concat_string_buffers);
    SUITE_ADD_TEST(suite, test_clear_string_buffer);
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
