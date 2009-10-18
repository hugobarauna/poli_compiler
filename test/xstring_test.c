#include <stdio.h>
#include "CuTest.h"
#include "../src/xstring.h"

void test_convert_an_string_to_integer(CuTest *tc)
{
    char *num = "100";
    int n;
    n = atoi(num);
    CuAssertIntEquals(tc, 100, n);
}

void test_convert_an_integer_to_string(CuTest *tc)
{
    char *str;
    str = itoa(100);
    CuAssertStrEquals(tc, "100", str);
    free(str);
}

void test_reverse_a_string(CuTest *tc)
{
    char *text1 = "hello world";
    char *text2 = "four";
    char *reversed1 = reverse(text1);
    char *reversed2 = reverse(text2);
    
    CuAssertStrEquals(tc, "dlrow olleh", reversed1);
    CuAssertStrEquals(tc, "ruof", reversed2);
}

void test_reverse1_to_reverse_the_string_itself(CuTest *tc)
{
    /*
     * The string block "str" is readonly on  C, so we must
     * create an "array" of chars with null char at end
     */
    char text1[] = { 'b', 'u', 't', '\0' };
    char text2[] = { 'd', 'o', '\0' };
    
    CuAssertStrEquals(tc, "but", text1);
    CuAssertStrEquals(tc, "do", text2);
    
    reverse1(text1);
    reverse1(text2);
    
    CuAssertStrEquals(tc, "tub", text1);
    CuAssertStrEquals(tc, "od", text2);
}
