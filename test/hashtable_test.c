#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../src/hashtable.h"

/* Helpers */
int hashing_helper(char *value);

void test_create_hashtable(CuTest *tc) {
  Hashtable *table = hashtable_new(10);
  CuAssertPtrNotNull(tc, table);

  for (int i = 0; i < 10; i++) {
    CuAssertPtrNull(tc, table->entries[i]);
  }

  hashtable_delete(table);
}

void test_insert_an_element(CuTest *tc) {
  Hashtable *table = hashtable_new(10);
  table->hashing_function = hashing_helper;

  hashtable_insert(table, "my_key", "my_value");
  CuAssertPtrNotNull(tc, table->entries[1]);

  hashtable_delete(table);
}

void test_get_an_element(CuTest *tc) {
  VALUE my_value;
  Hashtable *table = hashtable_new(10);

  table->hashing_function = hashing_helper;
  hashtable_insert(table, "my_key", "my_value");

  my_value = hashtable_get(table, "my_key");
  CuAssertStrEquals(tc, "my_value", my_value);

  hashtable_delete(table);
}

void test_get_an_element_in_an_empty_hashtable(CuTest *tc) {
  VALUE my_value;
  Hashtable *table = hashtable_new(10);

  my_value = hashtable_get(table, "my_key");

  CuAssertPtrNull(tc, my_value);
}

void test_get_an_element_with_colision(CuTest *tc) {
  VALUE my_value;
  Hashtable *table = hashtable_new(10);
  table->hashing_function = hashing_helper;

  hashtable_insert(table, "first_key", "first_value");
  hashtable_insert(table, "second_key", "second_value");

  my_value = hashtable_get(table, "second_key");
  CuAssertStrEquals(tc, "second_value", my_value);

  my_value = hashtable_get(table, "first_key");
  CuAssertStrEquals(tc, "first_value", my_value);

  hashtable_delete(table);
}

void test_get_an_null_element_with_colision(CuTest *tc) {
  VALUE my_value;
  Hashtable *table = hashtable_new(10);
  table->hashing_function = hashing_helper;

  hashtable_insert(table, "first_key", "first_value");
  my_value = hashtable_get(table, "second_key");

  CuAssertPtrNull(tc, my_value);
}

/* Helpers */
int hashing_helper(char *value) {
  return 1;
}
