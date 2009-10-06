#include <stdio.h>
#include <stdlib.h>

#include "error.h"

#ifndef __FATAL_ERROR__
#define __FATAL_ERROR__
void fatal_error(const char *msg)
{
    printf("%s\n", msg);
    exit(1);
}
#endif