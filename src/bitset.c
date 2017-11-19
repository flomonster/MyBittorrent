#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "bitset.h"


s_bitset *bitset_create(size_t size, char *data)
{
  size_t bcount = (size + (CHAR_BIT - 1)) / CHAR_BIT;
  s_bitset *ret = malloc(sizeof(*ret) + bcount);
  if (!ret)
    return NULL;

  ret->count = size;
  if (data)
    memcpy(&ret->data, data, bcount);
  else
    memset(&ret->data, 0, bcount);
  return ret;
}


bool bitset_get(s_bitset *bset, size_t i)
{
  return bset->data[i / CHAR_BIT] & 1 << (i % CHAR_BIT);
}


void bitset_set(s_bitset *bset, size_t i, bool val)
{
  unsigned char *obyte = &bset->data[i / CHAR_BIT];
  *obyte = (*obyte & ~(1 << i)) | val << i;
}


void bitset_free(s_bitset *bset)
{
  free(bset);
}
