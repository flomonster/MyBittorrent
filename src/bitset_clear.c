#include <string.h>

#include "bitset.h"


void bitset_clear(s_bitset *bitset)
{
  memset(bitset->data, 0, BITSET_BSIZE(bitset->size));
}
