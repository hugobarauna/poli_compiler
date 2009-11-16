#ifndef __STRING_BUFFER_H__
#define __STRING_BUFFER_H__

typedef struct _strbuf {
    int capacity;
    int length;
    char *buffer;
} StringBuffer;

StringBuffer *sbuf_new(int size);
void sbuf_delete(StringBuffer *sb);

StringBuffer *sbuf_append(StringBuffer *sb, const char *str);
StringBuffer *sbuf_append_char(StringBuffer *sb, char c);
StringBuffer *sbuf_concat(StringBuffer *sb1, StringBuffer *sb2);

void sbuf_clear(StringBuffer *sb);

char *to_str(StringBuffer *sb);

#endif