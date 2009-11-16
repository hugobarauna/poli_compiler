#include <stdlib.h>
#include <string.h>

#include "bufstream.h"
#include "error.h"

static void fill(BufferedInputStream *);

/*
 * Creates a new BufferedInputStream from a File Input Stream
 * with a specific buffer size.
 */
BufferedInputStream *buffered_input_stream_new(FILE *in, int size)
{
  if (in == NULL)
    fatal_error("Input File cannot be NULL.");
  if (size <= 0)
    fatal_error("Illegal buffer size, it should be greater than zero.");

  BufferedInputStream *buffered = (BufferedInputStream *)malloc(sizeof(BufferedInputStream));
  if (buffered == NULL)
    fatal_error("couldn't create a new BufferedInputStream");
  buffered->in = in;
  buffered->buffer_size = size;
  buffered->count = 0;
  buffered->pos = 0;
  buffered->markpos = -1;
  buffered->buf = (char *)calloc(buffered->buffer_size, sizeof(char));
  if (buffered->buf == NULL)
    fatal_error("couldn't alloc space for internal buffer");
  
  return buffered;
}

/*
 * Deletes a BufferInputStream instance and its internal buffer.
 * The file stream is closed.
 */
void buffered_input_stream_delete(BufferedInputStream *input)
{
  if (input == NULL) return;

  if (input->buf != NULL) {
    free(input->buf);
    input->buf = NULL;
  }
  if (input->in != NULL)
    fclose(input->in);
  free(input);
}

/*
 * Fill the internal character buffer. If there is no mark
 * setted the entire buffer is overwritten.
 *
 * If there is a marked position, the current marked buffer
 * is moved to the beginning of the buffer, and the rest of
 * buffer is filled with new data.
 *
 * In case the marked buffer is the entire buffer, the buffer
 * is growned to a factor of 2, in order to garantee that the
 * marked buffer is not lost.
 */
static void fill(BufferedInputStream *input)
{
  if (input->markpos < 0)
    input->pos = 0; /* no mark in buffer, fill entire buffer */
  else if (input->pos >= input->buffer_size) { /* no room left */
    if (input->markpos > 0) { /* move from mark to pos at the beginning of buffer */
      int sz = input->pos - input->markpos; /* number of chars to move */
      strncpy(input->buf, &input->buf[input->markpos], sz);
      input->pos = sz; /* fix the position */
      input->markpos = 0;
    } else { /* unsufficient space, grow buffer */
      input->buffer_size *= 2; /* double buffer size */
      input->buf = (char *)realloc(input->buf, input->buffer_size);
      if (input->buf == NULL)
	fatal_error("Couldn't grow the buffer size");
    }
  }
  input->count = input->pos; /* when there is a mark, we have some space used in buffer */
  size_t n = fread(&input->buf[input->pos], 1, input->buffer_size - input->pos, input->in);
  if (n > 0) /* if read more data */
    input->count = n + input->pos; /* update the count */
}

/*
 * Checks if the buffer is ended (EOF).
 */
int finished(BufferedInputStream *input)
{
  return input->buf[input->pos] == EOF;
}

/*
 * Reads a character from BufferedInputStream.
 */
int read(BufferedInputStream *input)
{
  /* Checks if needs to get more data */
  if (input->pos >= input->count) {
    fill(input);
    if (input->pos >= input->count)
      return EOF;
  }
  //printf("[%c]", input->buf[input->pos]);
  return (unsigned char)input->buf[input->pos++];
}

/*
 * Reads a line from input buffer.
 */
char *readline(BufferedInputStream *input)
{
  static char buf[1000];
  char *p;
  int c;
  p = buf;
  do {
    c = read(input);
    *p = (c == '\n' || c == EOF) ? '\0' : c;
  } while (*p++);
  return buf == '\0' ? NULL : strdup(buf);
}

/*
 * Unreads a character to buffer.
 *
 * If the current buffer is empty, than we start pushing back
 * the character at end of buffer, so we can do more pushes
 * without do many moves at it.
 * 
 * If we are positioned at the beginning of buffer we try to
 * shift right the data to create some rooms to put the char
 * back to buffer. If we can't, than we throw an error.
 *
 * To do safe pushes we must garantee that the buffer is marked
 * so we can put it back without have to deal with this issues.
 */
void unread(int ch, BufferedInputStream *input)
{
  if (input->count == 0) { /* buffer is empty */
    input->count = input->buffer_size;
    input->pos = input->count; /* put at end, so we can do more pushback */
  }
  else if (input->pos == 0) { /* at first position */
    int i;
    if (input->count >= input->buffer_size)
      fatal_error("push back char overflow");
    /* there is some room free */
    int offset = input->buffer_size - input->count;
    for (i = 0; i < input->count; ++i)
      input->buf[offset+i] = input->buf[i];
    input->pos = offset;    
  }
  input->buf[--input->pos] = (char) ch;
}

/*
 * Marks the buffer with the current position.
 *
 * Marking the buffer protects situations when the text being read
 * is greater that the buffer, allowing a rollback of text if needed.
 */
void mark(BufferedInputStream *input)
{
  input->markpos = input->pos;
}

/*
 * Removes the marked position in buffer.
 */
void unmark(BufferedInputStream *input)
{
  input->markpos = -1;
}
