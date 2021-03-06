#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../src/hashtable.h"
#include "../src/scope.h"

void test_create_scope(CuTest *tc) {  
  CuAssertIntEquals(tc, 0, scope_numbers());
  
  scope_new();
  
  CuAssertIntEquals(tc, 1, scope_numbers());
  scope_reset();
}


void test_reset_should_have_no_scope(CuTest *tc) {
  scope_new();
  
  CuAssertIntEquals(tc, 1, scope_numbers());
  scope_reset();
    
  CuAssertIntEquals(tc, 0, scope_numbers());
  scope_reset();
}

void test_insert_and_retrieve_entry_to_scope(CuTest *tc) {
  scope_new();
  scope_sym_table_insert("a", "A", NULL);
  
  CuAssertPtrNotNull(tc, scope_search_identifier("a"));
  scope_reset();
}

void test_do_not_find_entry_scope(CuTest *tc) {
  scope_new();
  
  CuAssertIntEquals(tc, 1, scope_numbers());
  CuAssertFalse(tc, is_identifier_declared("a"));
  CuAssertPtrNull(tc, scope_search_identifier("a"));
  
  scope_reset();
}

void test_search_on_parent_scope(CuTest *tc) {
  /* parent */
  scope_new();
  scope_sym_table_insert("a", "A", NULL);
  
  /* new scope */
  scope_new();
  
  CuAssertIntEquals(tc, 2, scope_numbers());
  CuAssertPtrNotNull(tc, scope_search_identifier("a"));
  scope_reset();
}