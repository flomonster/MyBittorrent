#include "btstats.h"
#include "torrent.h"


bool btstats_init(s_btstats *stats, s_torrent *tor)
{
  stats->tick = 0;
  stats->downloaded = 0;
  stats->uploaded = 0;
  return (stats->pieces_state = bitset_create(tor->nbpieces));
}


void btstats_destroy(s_btstats *stats)
{
  bitset_free(stats->pieces_state);
}
