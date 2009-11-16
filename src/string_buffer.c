#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "string_buffer.h"

StringBuffer *sbuf_new(int capacity)
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

void sbuf_delete(StringBuffer *sb)
{
    if (sb == NULL) return;
    free(sb->buffer);
    free(sb);
}

StringBuffer *sbuf_append_char(StringBuffer *sb, char c)
{
  if (sb == NULL) return sb;

  if (sb->length >= sb->capacity) {
    sb->buffer = realloc(sb->buffer, 2 * sb->capacity);
    if (sb->buffer == NULL)
      fatal_error("unable to grow string buffer");

    sb->capacity *= 2;
  }

  sb->buffer[sb->length++] = c;
  sb->buffer[sb->length] = '\0';

  return sb; 
}

StringBuffer *sbuf_append(StringBuffer *sb, const char *str)
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

StringBuffer *sbuf_concat(StringBuffer *sb1, StringBuffer *sb2)
{
    sbuf_append(sb1, to_str(sb2));
    return sb1;
}

void sbuf_clear(StringBuffer *sb)
{
    memset(sb->buffer, 0, sb->length);
    sb->length = 0;
}

char *to_str(StringBuffer *sb)
{
    if (sb == NULL) return '\0';
    return sb->buffer;
}
