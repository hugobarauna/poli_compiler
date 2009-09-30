#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../src/scanner.h"

void test_create_buffer(CuTest *tc)
{
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 10);
    CuAssertPtrNotNull(tc, b);
    CuAssertIntEquals(tc, 10, b->buf_size);
    CuAssertIntEquals(tc, 0, b->num_chars);
    delete_buffer(b);
}

void test_read_char_from_file(CuTest *tc)
{
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 10);
    int c = read();
    CuAssertCharEquals(tc, 'h', (char)c);
    delete_buffer(b);
}

void test_unread_char_to_buffer(CuTest *tc)
{
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 10);
    CuAssertIntEquals(tc, 0, b->num_chars);
    unread('h');
    CuAssertIntEquals(tc, 10, b->num_chars);
    delete_buffer(b);
}

void test_start_with_unread_the_returned_char_should_not_be_a_lexeme(CuTest *tc)
{
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 10);
    unread('h');
    CuAssertIntEquals(tc, 0, strlen(get_text()));
    delete_buffer(b);
}

void test_grow_buffer_when_token_is_bigger_than_buffer(CuTest *tc)
{
    int i, buffer_size = 2;
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, buffer_size);
    CuAssertIntEquals(tc, buffer_size, b->buf_size);
    for (i = 0; i < buffer_size+1; ++i)
        read();
    CuAssertIntEquals(tc, buffer_size*2, b->buf_size);
    delete_buffer(b);
}

void test_retrieve_token_readed(CuTest *tc)
{
    int i;
    const char *expected_word = "hello";
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 20);
    for (i = 0; i < strlen(expected_word); ++i)
        read();
    CuAssertStrEquals(tc, expected_word, get_text());
    delete_buffer(b);
}

void test_extract_token_get_text_should_be_empty(CuTest *tc)
{
    int i;
    const char *extracted_word = "hello";
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 20);
    for (i = 0; i < strlen(extracted_word); ++i)
        read();
    CuAssertStrEquals(tc, extracted_word, extract_text());
    CuAssertIntEquals(tc, 0, strlen(get_text()));
    delete_buffer(b);
}

void test_when_finished_reading_file_EOF_is_returned(CuTest *tc)
{
    int i, c;
    const char *text = "hello world";
    FILE *fp = fopen("scanner.in", "r");
    Buffer *b = new_buffer(fp, 20);
    for (i = 0; i < strlen(text); ++i)
        read();
    c = read();
    CuAssertIntEquals(tc, EOF, c);
    delete_buffer(b);
}