#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string_buffer.h"

static void fatal_error(const char *msg)
{
    printf("%s\n", msg);
    exit(1);
}

StringBuffer *new_default_string_buffer()
{
    return new_string_buffer(4096);
}

StringBuffer *new_string_buffer(int capacity)
{
    StringBuffer *sb = (StringBuffer *)malloc(sizeof(StringBuffer));
    if (sb == NULL)
        fatal_error("couldn't dynamic alloc string buffer.");
    
    sb->capacity = capacity;
    sb->length = 0;
    sb->buffer = (char *)calloc(sb->capacity + 1, sizeof(char));
    
    if (sb->buffer == NULL)
        fatal_error("couldn't dynamic alloc buffer for string_buffer.");
        
    return sb;
}

void delete_string_buffer(StringBuffer *sb)
{
    if (sb == NULL) return;
    free(sb->buffer);
    free(sb);
}

StringBuffer *append_str(StringBuffer *sb, const char *str)
{
    if (sb == NULL || str == NULL) return sb;

    size_t str_len = strlen(str);
    int space = sb->capacity - sb->length;
    while (str_len > space) {
        sb->buffer = realloc(sb->buffer, 2 * sb->capacity);
        if (sb->buffer == NULL)
            fatal_error("unable to grow string buffer");
        
        sb->capacity *= 2;
        space = sb->capacity - sb->length;
    }
    
    sb->length += str_len;
    strncat(sb->buffer, str, str_len);
    
    return sb;
}

StringBuffer *concat_strbuf(StringBuffer *sb1, StringBuffer *sb2)
{
    append_str(sb1, to_str(sb2));
    return sb1;
}

void clear_strbuf(StringBuffer *sb)
{
    memset(sb->buffer, 0, sb->length);
    sb->length = 0;
}

char *to_str(StringBuffer *sb)
{
    if (sb == NULL) return '\0';
    return sb->buffer;
}
