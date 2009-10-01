#include <stdio.h>
#include <string.h>

#include "CuTest.h"
#include "../src/hashtable.h"

void test_create_hashtable(CuTest *tc) {
  Hashtable *table = new_hashtable(10);
  CuAssertPtrNotNull(tc, table);
}
