#include <stdio.h>
#include "CuTest.h"
#include "../src/string_buffer.h"

void test_create_empty_string_buffer(CuTest *tc)
{
    StringBuffer *sb = sbuf_new(100);
    CuAssertIntEquals(tc, 100, sb->capacity);
    CuAssertIntEquals(tc, 0, sb->length);
    sbuf_delete(sb);
}

void test_append_string_to_buffer(CuTest *tc)
{
    const char *expected_string = "hello";
    StringBuffer *sb = sbuf_new(100);
    sbuf_append(sb, expected_string);
    CuAssertStrEquals(tc, expected_string, to_str(sb));
    sbuf_delete(sb);
}

void test_append_more_strings_to_buffer(CuTest *tc)
{
    StringBuffer *sb = sbuf_new(100);
    sbuf_append(sb, "hello ");
    sbuf_append(sb, "world");
    CuAssertStrEquals(tc, "hello world", to_str(sb));
    sbuf_delete(sb);
}

void test_append_string_bigger_than_buffer(CuTest *tc)
{
    StringBuffer *sb = sbuf_new(6);
    sbuf_append(sb, "hello ");
    sbuf_append(sb, "world");
    CuAssertIntEquals(tc, 12, sb->capacity);
    CuAssertStrEquals(tc, "hello world", to_str(sb));
    sbuf_delete(sb);
}

void test_concat_string_buffers(CuTest *tc)
{
    StringBuffer *sb1 = sbuf_new(100);
    StringBuffer *sb2 = sbuf_new(100);
    
    sbuf_append(sb1, "hello ");
    sbuf_append(sb2, "world");
    
    sbuf_concat(sb1, sb2);
    CuAssertStrEquals(tc, "hello world", to_str(sb1));
    
    sbuf_delete(sb1);
    sbuf_delete(sb2);
}

void test_clear_string_buffer(CuTest *tc)
{
    StringBuffer *sb = sbuf_new(100);
    
    sbuf_append(sb, "foo");
    CuAssertStrEquals(tc, "foo", to_str(sb));
    
    sbuf_clear(sb);
    CuAssertStrEquals(tc, "", to_str(sb));
    CuAssertIntEquals(tc, 0, sb->length);
    
    sbuf_delete(sb);
}