#include <stdio.h>

#include "CuTest.h"
#include "../src/semantic_actions.h"

void test_foo(CuTest *tc) {
  CuAssertIntEquals(tc, 1, 1);
}