#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdio.h>

#define END_OF_BUFFER       256
#define END_OF_BUFFER_CHAR  0

typedef struct buffer
{
    FILE *input_file;
    char *ch_buf; /* character buffer */
    char *buf_pos; /* current position in buffer */
    
    size_t buf_size; /* without EOB room */
    
    int num_chars; /* chars readed to buffer */
} Buffer;


Buffer *new_buffer(FILE *fp, int size);
void delete_buffer(Buffer *buf);

int read();
void unread(int c);

const char *get_text();
char *extract_text();
#endif