#include "bitset.h"
#include "decision.h"
#include "limits.h"
#include "peer.h"
#include "piece.h"
#include "torrent.h"

#include <stdlib.h>


s_piece *select_piece(s_peer *peer, s_torrent *tor)
{
  size_t *counter = calloc(sizeof(size_t), tor->nbpieces);

  s_peer *cur_peer = tor->peerlist.peers;
  for (; cur_peer; cur_peer = cur_peer->next)
    for (size_t i = 0; i < tor->nbpieces; i++)
      if (bitset_get(cur_peer->pieces, i))
        counter[i]++;

  bool has_missing = false;
  size_t best = 0;
  size_t score = SIZE_MAX;
  for (size_t i = 0; i < tor->nbpieces; i++)
  {
    has_missing |= tor->pieces[i].state != PIECE_AVAILABLE;
    if (bitset_get(peer->pieces, i)
        && tor->pieces[i].state == PIECE_MISSING
        && counter[i] < score)
    {
      best = i;
      score = counter[i];
    }
  }

  if (!has_missing)
    tor->state = T_COMPLETE;
  free(counter);

  if (score == SIZE_MAX)
    return NULL;
  return &tor->pieces[best];
}
