#include "bitset.h"

#include <stdlib.h>


char *bitset_to_string(s_bitset *bitset)
{
  char *buf = malloc(bitset->size + 1);
  if (!buf)
    return NULL;

  buf[bitset->size] = 0;
  for (size_t i = 0; i < bitset->size; i++)
    buf[i] = bitset_get(bitset, i) ? '1' : '0';

  return buf;
}
