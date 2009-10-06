#include "CuTest.h"
#include "../src/lexer.h"
#include "../src/string_buffer.h"
#include <stdio.h>
#include <stdlib.h>

/* http://cpp.pastebin.com/f154baf81 implementation */
static char* itoa (int n) {
    char s[17];
	char *u = NULL;
    int i=0, j=0, k=0; //s counter
    size_t u_len = 0;
    int is_negative = 0;

	if (n < 0) { //turns n positive
		n = (-1 * n); 
        u_len = 1; is_negative = 1;
	}
	
	do {
		s[i++]= n%10 + '0'; //conversion of each digit of n to char
		n -= n%10; //update n value
	} while ((n /= 10) > 0);

    u_len += i;
    u = calloc(0, i + u_len + 1); // +1 for NULL char
    if (u == NULL) {
        printf("couldn't dynamic alloc space for itoa.");
        exit(1);
    }
    
    if (is_negative)
        u[k++] = '-';
    
	for (j = i-1; j >= 0; j--)
		u[k++] = s[j];

	return u;
}

char *dump_token(Token *token) {
    if (token == NULL) return "";
    StringBuffer *sb = new_default_string_buffer();
    append_str(sb, "<");
    switch (token->class)
    {
        case ID:
        append_str(sb, "id, ");
        append_str(sb, token->value.word);
        break;
        case NUMBER:
        append_str(sb, "num, ");
        append_str(sb, itoa(token->value.number));
        break;
        case RESERVED:
        append_str(sb, "reserved, ");
        append_str(sb, token->value.word);
        break;
        case UNKNOWN:
        append_str(sb, "unknown, ");
        append_str(sb, token->value.word);
        break;
    }
    append_str(sb, ">");
    return to_str(sb);
}

void test_read_all_tokens(CuTest *tc) {
    Lexer *lexer = lexer_new("sample.fh");
    Token *token;
    StringBuffer *sb = new_default_string_buffer();
    
    do {
        token = lexer->read_token();
        append_str(sb, dump_token(token));
    } while (token != NULL);
    
    CuAssertStrEquals(tc, "<id, if><id, a><reserved, ==><num, 2><id, do><id, a><id, =><num, 12345><id, else><id, b><id, =><id, a><id, end>", to_str(sb));
    delete_string_buffer(sb);
    lexer_delete(lexer);
}

void test_new_lexer(CuTest *tc) {
    Lexer *lexer = lexer_new("sample.fh");
    //CuAssertPtrNotNull(tc, lexer->read_token);
    CuAssertPtrNotNull(tc, lexer->buffer);
    lexer_delete(lexer);
}

void _word_token(CuTest *tc) {
    Token *token = word_new(ID, "foo");
    CuAssertIntEquals(tc, ID, token->class);
    CuAssertStrEquals(tc, "foo", token->value.word);
    token_delete(token);
}

void test_number_token(CuTest *tc) {
    int expected_number = 1234;
    Token *token = number_new(expected_number);
    CuAssertIntEquals(tc, NUMBER, token->class);
    CuAssertIntEquals(tc, expected_number, token->value.number);
    token_delete(token);
}

void test_dump_token_word(CuTest *tc) {
    Token *token = word_new(ID, "foo");
    CuAssertStrEquals(tc, "<id, foo>", dump_token(token));
    token_delete(token);
}

void test_should_return_NULL_if_there_is_no_more_token(CuTest *tc) {
    Lexer *lexer = lexer_new("empty.fh");
    Token *token = lexer->read_token();
    CuAssertPtrNull(tc, token);
    lexer_delete(lexer);
}