#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "bufstream.h"
#define BUFFER_SIZE 300

int main(int argc, char **argv) {
  static FILE *fp;
  static BufferedInputStream *in;  

  if (argv[1] == NULL) {
    printf("In order to generate the mvn code, you need to pass an argument to the compiler.\n");
    printf("Please, do something like: ./bin/compiler my_source.fh\n");
    return 0;
  }
  
	fp = fopen(argv[1], "r");
	if (fp == NULL)
    printf("It seems that the file %s does not exist. Please, check this.\n", argv[1]);
  else {
    in = buffered_input_stream_new(fp, BUFFER_SIZE);
    parse(in);
    printf("\n\nYour program was compiled, you can check the output at the out.asm file\n");
  }

  return 0;
}