#include <stdio.h>
#include "CuTest.h"
#include "../src/bufstream.h"

#define BUFFER_SIZE 5

static FILE *fp;
static BufferedInputStream *in;

static void setup()
{
    fp = fopen("scanner.in", "r");
    in = buffered_input_stream_new(fp, BUFFER_SIZE);
}

static void teardown()
{
    buffered_input_stream_delete(in);
}

void test_create_buffered_input_stream(CuTest *tc)
{
    setup();
    CuAssertPtrNotNull(tc, in);
    
    CuAssertIntEquals(tc, 0, in->pos);
    CuAssertIntEquals(tc, 0, in->count);
    CuAssertIntEquals(tc, BUFFER_SIZE, in->buffer_size);
    CuAssertIntEquals(tc, -1, in->markpos);
    teardown();
}

void test_read_a_char_from_file(CuTest *tc)
{
    int ch;
    setup();
    
    ch = read(in);
    
    CuAssertCharEquals(tc, 'h', ch);
    CuAssertIntEquals(tc, in->buffer_size, in->count);
    
    teardown();
}

void test_unread_a_char_from_file(CuTest *tc)
{
    int ch;
    setup();
    ch = read(in);
    unread(ch, in);
    CuAssertIntEquals(tc, ch, read(in));
    teardown();
}

void test_unread_with_empty_buffer_should_be_added_at_end_of_buffer(CuTest *tc)
{
    int expected = 'h';
    setup();
    unread(expected, in);
    /* 
     * starting with an empty buffer, the char should be added
     * to the end of string
     */
    CuAssertIntEquals(tc, in->buffer_size - 1, in->pos);
    CuAssertIntEquals(tc, expected, read(in));
    
    teardown();
}

void test_when_end_of_buffer_is_reach_the_buffer_do_not_grow(CuTest *tc)
{
    int i;
    setup();
    for (i = 0; i < BUFFER_SIZE; ++i)
        read(in);
    CuAssertIntEquals(tc, BUFFER_SIZE, in->buffer_size);
    
    read(in);
    CuAssertIntEquals(tc, BUFFER_SIZE, in->buffer_size);
    teardown();
}

void test_grow_buffer_when_marked(CuTest *tc)
{
    int i;
    setup();
    mark(in);
    for (i = 0; i < BUFFER_SIZE; ++i)
        read(in);
    CuAssertIntEquals(tc, BUFFER_SIZE, in->buffer_size);
    
    read(in);
    CuAssertIntEquals(tc, 2*BUFFER_SIZE, in->buffer_size);
    teardown();
}