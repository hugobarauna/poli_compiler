#include <stdio.h>

#include "CuTest.h"
#include "../src/semantic_actions.h"

static void setup() {
  semantic_initialize();
}

void test_generate_label(CuTest *tc) {
  CuAssertStrEquals(tc, "VAR_0", generate_label(0, L_VARIABLE));
  CuAssertStrEquals(tc, "K_7", generate_label(7, L_CONSTANT));
  CuAssertStrEquals(tc, "TEMP_10", generate_label(10, L_TEMP));
  CuAssertStrEquals(tc, "IF_10", generate_label(10, L_IF));
  CuAssertStrEquals(tc, "ELSE_10", generate_label(10, L_ELSE));
  CuAssertStrEquals(tc, "ENDIF_10", generate_label(10, L_ENDIF));
  CuAssertStrEquals(tc, "WHILE_10", generate_label(10, L_WHILE));
  CuAssertStrEquals(tc, "ENDWHILE_10", generate_label(10, L_ENDWHILE));
  //CuAssertStrEquals(tc, "ENDWHILE_10", generate_label(10, L_FUNC));
  //CuAssertStrEquals(tc, "ENDWHILE_10", generate_label(10, L_FUNC_VAR));
}