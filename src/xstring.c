#include <string.h>
#include <stdlib.h>

#include "xstring.h"

char *reverse(const char *str)
{
  int len;
  char const *src;
  char *reversed, *dest;
  
  len = strlen(str);
  if (len < 2) return strdup(str); /* if len is 0 or 1, no need to reverse */

  reversed = (char *)calloc(len + 1, sizeof(char));
  if (reversed == NULL) return NULL;

  src = str;
  dest = &reversed[len];
  for ( ; *src; )
    *--dest = *src++;

  return reversed;
}

void reverse1(char *str)
{
  char *src, *dest;
  int len;
  char aux;

  len = strlen(str);
  src = str;
  dest = &str[len-1];

  for (; src < dest; ) {
    aux = *dest;
    *dest-- = *src;
    *src++ = aux;
  }
}
/*
int main(void)
{
  char foo[] = { 'b','a','r','c','d','\0'};
  char bar[] = {'1','2','3','4', '\0'};
  FILE *fp = fopen("tokens.g", "r");
  BufferedInputStream *stream = buffered_input_stream_new(fp, 100);
  printf("token: %s\n", reverse(readline(stream)));
  printf("token: %s\n", readline(stream));
  printf("token: %s\n", readline(stream));
  printf("token: %s\n", readline(stream));
  printf("token: %s\n", readline(stream));

  printf("token: %s\n", reverse("hello world"));

  printf("foo: %p\n", foo);
  reverse1(foo);
  reverse1(bar);
  printf("%s\n", foo);
  printf("%s\n", bar);

  return 0;
}
*/
