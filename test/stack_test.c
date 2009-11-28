#include <stdio.h>
#include "CuTest.h"
#include "../src/stack.h"

static Stack s;

static void setup() {
  stack_clean(&s);
}

void test_stack_clean(CuTest *tc) {
  Stack s;
  stack_clean(&s);
  CuAssertIntEquals(tc, -1, s.top);
}

void test_stack_push(CuTest *tc) {
  setup();
  stack_push("item", &s);
  CuAssertIntEquals(tc, 0, s.top);
}

void test_stack_pop(CuTest *tc) {
  setup();
  char* item;

  stack_push("word", &s);
  item = stack_pop(&s);
  CuAssertStrEquals(tc, "word", item);
  CuAssertIntEquals(tc, -1, s.top);
}

void test_stack_lookup(CuTest *tc) {
  setup();
  char* item;

  stack_push("word", &s);
  item = stack_lookup(&s);
  CuAssertStrEquals(tc, "word", item);
  CuAssertIntEquals(tc, 0, s.top);
}

void test_new_stack_empty(CuTest *tc) {
  setup();
  CuAssertTrue(tc, stack_empty(&s));
}

void test_stack_empty(CuTest *tc) {
  setup();

  stack_push("word", &s);
  stack_pop(&s);
  CuAssertTrue(tc, stack_empty(&s));
}