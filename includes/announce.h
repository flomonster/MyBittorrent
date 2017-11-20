#pragma once

#include <stdbool.h>

#include "torrent.h"


typedef struct announce
{
  s_dbuf *raw;
  s_bdata *bencoded;
} s_announce;


s_announce *tracker_announce(s_torrent *tor, const char *ev);
void announce_free(s_announce *announce);
