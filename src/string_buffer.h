#ifndef __STRING_BUFFER_H__
#define __STRING_BUFFER_H__

typedef struct string_buffer {
    int capacity;
    int length;
    char *buffer;
} StringBuffer;

StringBuffer *new_default_string_buffer();
StringBuffer *new_string_buffer(int size);
void delete_string_buffer(StringBuffer *sb);

StringBuffer *append_str(StringBuffer *sb, const char *str);
StringBuffer *concat_strbuf(StringBuffer *sb1, StringBuffer *sb2);

void clear_strbuf(StringBuffer *sb);

char *to_str(StringBuffer *sb);

#endif