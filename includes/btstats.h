#pragma once


#include "bitset.h"

#include <stddef.h>


typedef struct btstats
{
  size_t tick;
  size_t downloaded;
  size_t uploaded;
  s_bitset *pieces_state;
} s_btstats;


struct torrent;

bool btstats_init(s_btstats *stats, struct torrent *tor);
void btstats_destroy(s_btstats *stats);
