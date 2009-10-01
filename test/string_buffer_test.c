#include <stdio.h>
#include "CuTest.h"
#include "../src/string_buffer.h"

void test_create_empty_string_buffer(CuTest *tc)
{
    StringBuffer *sb = new_string_buffer(100);
    CuAssertIntEquals(tc, 100, sb->capacity);
    CuAssertIntEquals(tc, 0, sb->length);
    delete_string_buffer(sb);
}

void test_append_string_to_buffer(CuTest *tc)
{
    const char *expected_string = "hello";
    StringBuffer *sb = new_default_string_buffer();
    append_str(sb, expected_string);
    CuAssertStrEquals(tc, expected_string, to_str(sb));
    delete_string_buffer(sb);
}

void test_append_more_strings_to_buffer(CuTest *tc)
{
    StringBuffer *sb = new_default_string_buffer();
    append_str(sb, "hello ");
    append_str(sb, "world");
    CuAssertStrEquals(tc, "hello world", to_str(sb));
    delete_string_buffer(sb);
}

void test_append_string_bigger_than_buffer(CuTest *tc)
{
    StringBuffer *sb = new_string_buffer(6);
    append_str(sb, "hello ");
    append_str(sb, "world");
    CuAssertIntEquals(tc, 12, sb->capacity);
    CuAssertStrEquals(tc, "hello world", to_str(sb));
    delete_string_buffer(sb);
}

void test_concat_string_buffers(CuTest *tc)
{
    StringBuffer *sb1 = new_default_string_buffer();
    StringBuffer *sb2 = new_default_string_buffer();
    
    append_str(sb1, "hello ");
    append_str(sb2, "world");
    
    concat_strbuf(sb1, sb2);
    CuAssertStrEquals(tc, "hello world", to_str(sb1));
    
    delete_string_buffer(sb1);
    delete_string_buffer(sb2);
}

void test_clear_string_buffer(CuTest *tc)
{
    StringBuffer *sb = new_default_string_buffer();
    
    append_str(sb, "foo");
    CuAssertStrEquals(tc, "foo", to_str(sb));
    
    clear_strbuf(sb);
    CuAssertStrEquals(tc, "", to_str(sb));
    CuAssertIntEquals(tc, 0, sb->length);
    
    delete_string_buffer(sb);
}