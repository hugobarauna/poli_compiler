#include "CuTest.h"
#include "../src/lexer.h"
#include "../src/string_buffer.h"
#include "../src/bufstream.h"
#include "../src/xstring.h"
#include <stdio.h>
#include <stdlib.h>

static char *token_to_s(Token *token) {
    if (token == NULL) return "";
    StringBuffer *sb = sbuf_new(1024);
    sbuf_append(sb, "<");
    switch (token->class)
    {
        case IDENTIFIER:
        sbuf_append(sb, "id, ");
        sbuf_append(sb, token->value);
        break;
        case NUMBER:
        sbuf_append(sb, "num, ");
        sbuf_append(sb, token->value);
        break;
        default:
        sbuf_append(sb, token->value);
        break;
    }
    sbuf_append(sb, ">");
    return to_str(sb);
}

void test_read_all_tokens(CuTest *tc) {
    FILE *fp = fopen("sample.fh", "r");
    BufferedInputStream *in = buffered_input_stream_new(fp, DEFAULT_BUFFER_SIZE);
    Token *token;
    StringBuffer *sb = sbuf_new(1024);
    
    do {
        token = next_token(in);
        sbuf_append(sb, token_to_s(token));
    } while (token != NULL);
    
    CuAssertStrEquals(tc, "<if><id, a><==><num, 2><do><id, a><=><num, 12345><else><id, b><=><id, a><end>", to_str(sb));
    sbuf_delete(sb);
}
