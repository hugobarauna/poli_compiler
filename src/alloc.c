#include <stdlib.h>

#include "alloc.h"
#include "error.h"

/*
 * Allocates properly a table with specified sizes.
 */
void **talloc(int row, int col, size_t size)
{
  int i;
  void **tbl = malloc(row * size);
  if (tbl == NULL)
    fatal_error("Couldn't alloc memory for table");

  for (i = 0; i < row; ++i) { /* creating each column for table */
    tbl[i] = calloc(col, size);
    if (tbl[i] == NULL)
      fatal_error("Couln't alloc memory for table");
  }
  return tbl;
}
