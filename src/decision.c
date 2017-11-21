#include "bitset.h"
#include "decision.h"
#include "limits.h"
#include "peer.h"
#include "piece.h"
#include "torrent.h"


size_t select_piece(s_peer *peer, s_torrent *tor)
{
  size_t counter[tor->nbpieces];
  for (size_t i = 0; i < tor->nbpieces; i++)
    counter[i] = 0;

  s_peer *cur_peer = tor->peerlist.peers;
  while (cur_peer)
  {
    for (size_t i = 0; i < tor->nbpieces; i++)
      if (bitset_get(cur_peer->pieces, i))
        counter[i]++;
    cur_peer = cur_peer->next;
  }

  size_t best = 0;
  size_t score = UINT_MAX;
  for (size_t i = 0; i < tor->nbpieces; i++)
    if (bitset_get(peer->pieces, i)
        && tor->pieces[i].state == PIECE_MISSING
        && counter[i] < score)
    {
      best = i;
      score = counter[i];
    }

  return best;
}
