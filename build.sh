#!/bin/bash

echo "Compiling the compiler for people that does not have ruby (argh!)..."

gcc -std=c99 -Wall -O2 -g -c src/compiler.c

gcc -std=c99 -Wall -O2 -g -c src/parser.c

gcc -std=c99 -Wall -O2 -g -c src/lexer.c

gcc -std=c99 -Wall -O2 -g -c src/string_buffer.c

gcc -std=c99 -Wall -O2 -g -c src/error.c

gcc -std=c99 -Wall -O2 -g -c src/bufstream.c

gcc -std=c99 -Wall -O2 -g -c src/xstring.c

gcc -std=c99 -Wall -O2 -g -c src/alloc.c

gcc -std=c99 -Wall -O2 -g -c src/hashtable.c

gcc -std=c99 -Wall -O2 -g -c src/stack.c

gcc -std=c99 -Wall -O2 -g -c src/semantic_actions.c

gcc -std=c99 -Wall -O2 -g -c src/scope.c

gcc -std=c99 -Wall -O2 -g compiler.o parser.o lexer.o string_buffer.o error.o bufstream.o xstring.o alloc.o hashtable.o stack.o semantic_actions.o scope.o -o bin/compiler

echo "The compiler was compiled to bin/compiler."