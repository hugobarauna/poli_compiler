#include <stdio.h>
#include "CuTest.h"
#include "../src/parser.h"
#include "../src/bufstream.h"

#define BUFFER_SIZE 100

static FILE *fp;
static BufferedInputStream *in;

static void setup() {
	fp = fopen("source_for_parser_test.fh", "r");
	in = buffered_input_stream_new(fp, BUFFER_SIZE);
}

static void teardown() {
    buffered_input_stream_delete(in);
}

void test_assigment_recognition(CuTest *tc) {
  setup();
  CuAssertIntEquals(tc, 1, parse(in));
  teardown();
}