#pragma once


#include "bitset.h"

#include <stddef.h>


typedef struct btstats
{
  size_t downloaded;
  size_t uploaded;
  s_bitset *pieces_state;
} s_btstats;
