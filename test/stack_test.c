#include <stdio.h>
#include "CuTest.h"
#include "../src/stack.h"

static Stack s;

void setup() {
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
  char* item;

  setup();
  stack_push("word", &s);
  item = stack_pop(&s);
  CuAssertStrEquals(tc, "word", item);
  CuAssertIntEquals(tc, -1, s.top);
}