#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

#define EOB_IS_END_OF_FILE      0
#define EOB_IS_END_OF_BUFFER    1

/* from current buffer */
static FILE *current_input = NULL;
static char *current_char = NULL; /* current char */
static char next_char; /* */
//static char *ch_buf_pos = NULL; /* */
static char *begin_lexeme_ptr = NULL; /* */
static int num_chars; /* number of chars readed */
static Buffer *current_buffer = NULL;

static void fatal_error(const char *msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

static void dump_buffer(const char* buffer, int size)
{
    int i;
    printf("Dump buffer: ");
    for (i = 0; i < size; ++i)
        printf("<%x>", buffer[i]);
    printf("\n");
}

static void load_buffer()
{
    num_chars = current_buffer->num_chars;
    begin_lexeme_ptr = current_char = current_buffer->buf_pos;
    current_input = current_buffer->input_file;
    next_char = *current_char; /* no readed char */
}

static void flush_buffer(Buffer *buf)
{
    if (buf == NULL) return;
    
    buf->num_chars = 0;
    
    buf->ch_buf[0] = END_OF_BUFFER_CHAR;
    buf->ch_buf[1] = END_OF_BUFFER_CHAR;
    
    buf->buf_pos = &buf->ch_buf[0];
    
    if (buf == current_buffer)
        load_buffer(buf);
}

static void init_buffer(Buffer *buf, FILE *fp)
{
    buf->input_file = fp;
    flush_buffer(buf);
}

Buffer *new_buffer(FILE *fp, int size)
{
    Buffer *b;
    if (fp == NULL) return NULL;
    
    b = (Buffer *) malloc(sizeof(Buffer));
    
    if (b == NULL)
        fatal_error("couldn't dynamic alloc a Buffer.");
    
    b->buf_size = size;
    
    /* ch_buf has to be 2 chars longer than the size
     * in order to add 2 EOB chars
     */
     b->ch_buf = (char *) malloc(b->buf_size + 2);
     if (b->ch_buf == NULL)
         fatal_error("couldn't dynamic alloc the char buffer.");

     current_buffer = b;        
     init_buffer(b, fp);

     return b;
}

void delete_buffer(Buffer *buf)
{
    if (buf == NULL) return;
    free(buf->ch_buf);
    free(buf);
}



char *extract_text() {
    char *lexeme = strdup(begin_lexeme_ptr);
    begin_lexeme_ptr = current_char;
    return lexeme;
}

const char *get_text() {
    return begin_lexeme_ptr;
}

static int get_next_buffer()
{
    char *dest = current_buffer->ch_buf;
    char *source = begin_lexeme_ptr;
    int status, i;
    //printf("\t\t--- begin get_next_buffer() ---\n");
    /* current_char should be pointing to the second EOB */
    if (current_char > &current_buffer->ch_buf[num_chars+1])
        fatal_error("*** char buffer overflow occured. ***");
    
    /* first we need to shift the current lexeme being match to the beginning of the buffer */
    /* -1 because we have two EOBs */
    int number_to_move = (int) (current_char - begin_lexeme_ptr) - 1;
    //printf("\t\tnumber_to_move: %d\n", number_to_move);
    for (i = 0; i < number_to_move; ++i) {
        // *(dest++) = *(source++);
        //printf("\t\tdest: %c (0x%x) <- source: %c (0x%x)\n", *dest, *dest, *source, *source);
        *dest++ = *source++;
    }
    
    int num_to_read = current_buffer->buf_size - number_to_move;
    //printf("\t\tnum_to_read: %d\n", num_to_read);
    
    /* the current lexeme being match is the whole or bigger than the char buffer */
    while (num_to_read <= 0) {
        //printf("\t\twe need more room to continue reading\n");
        
        Buffer *b = current_buffer;
        
        /* we need to keep the offset of lexeme, because we will 
         * realloc the buffer, so the address will change */
        int c_buf_p_offset = current_char - b->ch_buf;
        
        if (b->buf_size*2 <= 0)
            fatal_error("can't grow up a 0 length buffer.");
        
        b->buf_size *= 2;
        
        b->ch_buf = (char *) realloc(b->ch_buf, b->buf_size + 2);
        if (b->ch_buf == NULL)
            fatal_error("couldn't grow the char buffer.");
        
        /* fix the current char */
        current_char = &b->ch_buf[c_buf_p_offset];
        
        num_to_read = b->buf_size - number_to_move;
    }
    
    /* read more data to buffer */
    num_chars = fread(&current_buffer->ch_buf[number_to_move], 1, num_to_read, current_input);
    
    //printf("\t\tnum_chars readed: %d\n", num_chars);
    current_buffer->num_chars = num_chars;
    
    if (num_chars == 0)
        status = EOB_IS_END_OF_FILE;
    else
        status = EOB_IS_END_OF_BUFFER;
    
    num_chars += number_to_move; /* correct the number of chars in buffer */
    current_buffer->ch_buf[num_chars] = END_OF_BUFFER_CHAR;
    current_buffer->ch_buf[num_chars+1] = END_OF_BUFFER_CHAR;
    
    begin_lexeme_ptr = &current_buffer->ch_buf[0];
    
    //printf("\t\t---- end get_next_buffer() ----\n");
    return status;
}

int read()
{
    int ch;
    //printf("--- begin read_char() ---\n");
    //printf("ch_buf: %p, current_char: %p\n", current_buffer->ch_buf, current_char);

    //printf("*current_char: %c (%x) <- next_char: %c (%x)\n", *current_char, *current_char, next_char, next_char);
    *current_char = next_char;
    
    if (*current_char == END_OF_BUFFER_CHAR)
    {
        //printf("\tcurrent_char is END_OF_BUFFER_CHAR\n");
        if (current_char < &current_buffer->ch_buf[num_chars]) {
            //printf("is a true EOF\n");
            *current_char = '\0';
        }
        else
        {
            //printf("\tgetting next buffer\n");
            int offset = current_char - begin_lexeme_ptr;
            //printf("\toffset: %d\n", offset);
            ++current_char; // go to second EOB
            switch (get_next_buffer())
            {
                case EOB_IS_END_OF_FILE:
                {
                return EOF;
                }
                case EOB_IS_END_OF_BUFFER:
                // correct the current_char position
                current_char = begin_lexeme_ptr + offset;
                break;
            }
        }
    }
    
    ch = *(unsigned char *) current_char;
    next_char = *++current_char;
    *current_char = '\0'; /* clean char readed */
    //printf("---- end read_char() ----\n");
    return ch;
}

void unread(int ch) 
{
    if (ch == EOF) ch = '\0';
    char *copy;

    /* undo the setting up of lexeme with \0 */
    copy = current_char;
    *copy = next_char; // put char back
    
    char *bp = begin_lexeme_ptr;
    //printf("ch_buf: %p, current_char: %p\n", current_buffer->ch_buf, current_char);
    
    *copy = next_char;
    
    //printf("copy: %p, current_buffer->ch_buf: %p\n", copy, current_buffer->ch_buf);
    if (copy < current_buffer->ch_buf+2)
    {
        printf("\t--- try to create a room for char ---\n");
        int number_to_move = num_chars + 2;
        printf("\tnumber_to_move: %d\n", number_to_move);
        /* starts from end of buffer */
        char *dest = &current_buffer->ch_buf[current_buffer->buf_size+2];
        char *source = &current_buffer->ch_buf[number_to_move];
        
        while (source > current_buffer->ch_buf) {
            printf("\t*dest = %c, *source = %c\n", *dest, *source);
            *--dest = *--source;
        }
        
        int offset = (int) (dest - source);
        printf("\toffset: %d\n", offset);
        copy += offset;
        bp += offset;
        printf("\tcopy: %p, bp: %p\n", copy, bp);
        current_buffer->num_chars = num_chars = current_buffer->buf_size;
        
        if (copy < current_buffer->ch_buf+2)
            fatal_error("push char back overflow.");
        printf("\t--- end try to create a room for char ---\n");
    }
    
    next_char = ch;
    *--copy = '\0';
    begin_lexeme_ptr = bp;
    current_char = copy;
}
