#include <stdio.h>
#include "CuTest.h"
#include "../src/alloc.h"

void test_table_allocation(CuTest *tc)
{
    char **table = (char **)talloc(3, 3, sizeof(char));
    table[0][0] = '0';
    table[1][1] = '1';
    table[2][2] = '2';
    
    CuAssertCharEquals(tc, '0', table[0][0]);
    CuAssertCharEquals(tc, '1', table[1][1]);
    CuAssertCharEquals(tc, '2', table[2][2]);
    
    free(table);
}