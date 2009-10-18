#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void fatal_error(const char *msg)
{
    printf("%s\n", msg);
    exit(EXIT_FAILURE);
}
