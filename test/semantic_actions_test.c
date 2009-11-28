#include <stdio.h>

#include "CuTest.h"
#include "../src/semantic_actions.h"

static void setup() {
  sym_table_initialize();
}

void test_sym_table_insert_and_get(CuTest *tc) {
  setup();
  SymTableEntry* entry;
  VariableDescriptor* variable = (VariableDescriptor* ) malloc (sizeof(VariableDescriptor));

  sym_table_insert("myVar", "V0", variable);
  entry = sym_table_get("myVar");

  CuAssertStrEquals(tc, "myVar", entry->id_name);
  CuAssertStrEquals(tc, "V0", entry->label);
  CuAssertPtrEquals(tc, variable, entry->descriptor);
}

void test_sym_table_get_miss(CuTest *tc) {
  setup();
  SymTableEntry* entry;
  entry = sym_table_get("foo");
  CuAssertPtrNull(tc, entry);
}

void test_generate_label(CuTest *tc) {
  // char a[] = {'V', '0', '\0', 'a'};   
  // CuAssertStrEquals(tc, "V0", generate_label(0, L_VARIABLE));
  // CuAssertStrEquals(tc, "V0", a);
  CuAssertStrEquals(tc, "C7", generate_label(7, L_CONSTANT));
}