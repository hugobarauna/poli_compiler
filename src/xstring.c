#include <string.h>
#include <stdlib.h>

#include "xstring.h"
#include "error.h"

/* http://cpp.pastebin.com/f154baf81 implementation */
char* itoa (int n) {
    char s[17];
	char *u = NULL;
    int i=0, j=0, k=0; //s counter
    size_t u_len = 0;
    int is_negative = 0;

	if (n < 0) { //turns n positive
		n = (-1 * n); 
        u_len = 1; is_negative = 1;
	}
	
	do {
		s[i++]= n%10 + '0'; //conversion of each digit of n to char
		n -= n%10; //update n value
	} while ((n /= 10) > 0);

    u_len += i;
    u = calloc(0, i + u_len + 1); // +1 for NULL char
    if (u == NULL)
        fatal_error("Couldn't alloc space");
    
    if (is_negative)
        u[k++] = '-';
    
	for (j = i-1; j >= 0; j--)
		u[k++] = s[j];

	return u;
}

int atoi(const char *str)
{
    int number = 0;
    const char *p;
    p = str;
    for (; *p; p++) {
        if ('0' > *p || *p > '9')
            fatal_error("Invalid number. All characters must be 0-9.");
        number = 10 * number + (*p - '0');
    }
    return number;
}

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
