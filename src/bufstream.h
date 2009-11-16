#ifndef __BUFSTREAM_H__
#define __BUFSTREAM_H__

#include <stdio.h>

#define DEFAULT_BUFFER_SIZE 8196

typedef struct buffered_input_stream {
  FILE *in;
  size_t buffer_size;
  char *buf;
  int count;
  int pos;
  int markpos;
} BufferedInputStream;

BufferedInputStream *buffered_input_stream_new(FILE *, int);
void buffered_input_stream_delete(BufferedInputStream *);
int read(BufferedInputStream *);
int finished(BufferedInputStream *);
void unread(int, BufferedInputStream *);
char *readline(BufferedInputStream *);
void mark(BufferedInputStream *);
void unmark(BufferedInputStream *);

#endif