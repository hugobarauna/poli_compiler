#include <stdio.h>

#include "CuTest.h"
#include "../src/semantic_actions.h"

void test_sym_table_insert_and_get(CuTest *tc) {
  SymTableEntry* entry;
  sym_table_initialize();
  
  sym_table_insert("myVar", "int", VARIABLE);
  entry = sym_table_get("myVar");

  CuAssertStrEquals(tc, "myVar", entry->id_name);
  CuAssertStrEquals(tc, "int", entry->id_type);
  CuAssertIntEquals(tc, VARIABLE, entry->descriptor);
}