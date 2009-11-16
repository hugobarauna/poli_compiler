#include <stdio.h>

/* This is auto-generated code. Edit at your own peril. */

#include "CuTest.h"

extern void test_create_hashtable(CuTest*);
extern void test_insert_an_element(CuTest*);
extern void test_get_an_element(CuTest*);
extern void test_get_an_element_in_an_empty_hashtable(CuTest*);
extern void test_get_an_element_with_colision(CuTest*);
extern void test_get_an_null_element_with_colision(CuTest*);
extern void test_internal_key_should_be_inside_range_of_table_indexes(CuTest*);


void RunAllTests(void) 
{
    CuString *output = CuStringNew();
    CuSuite* suite = CuSuiteNew();

    SUITE_ADD_TEST(suite, test_create_hashtable);
    SUITE_ADD_TEST(suite, test_insert_an_element);
    SUITE_ADD_TEST(suite, test_get_an_element);
    SUITE_ADD_TEST(suite, test_get_an_element_in_an_empty_hashtable);
    SUITE_ADD_TEST(suite, test_get_an_element_with_colision);
    SUITE_ADD_TEST(suite, test_get_an_null_element_with_colision);
    SUITE_ADD_TEST(suite, test_internal_key_should_be_inside_range_of_table_indexes);


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
