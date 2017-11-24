#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "bitset.h"


s_bitset *bitset_create(size_t size)
{
  size_t bcount = BITSET_BSIZE(size);
  s_bitset *ret = malloc(sizeof(*ret) + bcount);
  if (!ret)
    return NULL;

  ret->size = size;
  memset(&ret->data, 0, bcount);
  return ret;
}


bool bitset_update(s_bitset *bset, char *buf, size_t size)
{
  if (size > BITSET_BSIZE(bset->size))
    return true;

  memcpy(bset->data, buf, size);
  return false;
}



#define BIT_OFF(I) (CHAR_BIT - 1 - (I) % CHAR_BIT)
#define BYTE_OFF(I) ((I) / CHAR_BIT)

bool bitset_get(s_bitset *bset, size_t i)
{
  return bset->data[BYTE_OFF(i)] & 1 << BIT_OFF(i);
}


void bitset_set(s_bitset *bset, size_t i, bool val)
{
  unsigned char *obyte = &bset->data[BYTE_OFF(i)];
  *obyte = (*obyte & ~(1 << BIT_OFF(i))) | val << BIT_OFF(i);
}


void bitset_free(s_bitset *bset)
{
  free(bset);
}
