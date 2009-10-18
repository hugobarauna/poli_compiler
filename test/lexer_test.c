#include "CuTest.h"
#include "../src/lexer.h"
#include "../src/string_buffer.h"
#include "../src/bufstream.h"
#include "../src/xstring.h"
#include <stdio.h>
#include <stdlib.h>

static char *token_to_s(Token *token) {
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
    FILE *fp = fopen("sample.fh", "r");
    BufferedInputStream *in = buffered_input_stream_new(fp, DEFAULT_BUFFER_SIZE);
    Token *token;
    StringBuffer *sb = new_default_string_buffer();
    
    init_lexer();
    
    do {
        token = read_token(in);
        append_str(sb, token_to_s(token));
    } while (token != NULL);
    
    CuAssertStrEquals(tc, "<id, if><id, a><reserved, ==><num, 2><id, do><id, a><reserved, =><num, 12345><id, else><id, b><reserved, =><id, a><id, end>", to_str(sb));
    delete_string_buffer(sb);
    destroy_lexer();
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
    CuAssertStrEquals(tc, "<id, foo>", token_to_s(token));
    token_delete(token);
}

void test_should_return_NULL_if_there_is_no_more_token(CuTest *tc) {
    //Lexer *lexer = lexer_new("empty.fh");
    //Token *token = lexer->read_token();
    //CuAssertPtrNull(tc, token);
    //lexer_delete(lexer);
}