#pragma once

#include "peer.h"


struct announce;
struct torrent;


typedef struct peerlist
{
  s_peer *peers;
  size_t nbpeers;
} s_peerlist;

bool peerlist_init(s_peerlist *peerlist, struct torrent *tor,
                   struct announce *announce);
void peerlist_print(FILE *f, s_peerlist *peerlist);
void peerlist_destroy(s_peerlist *peerlist);
bool peerlist_append(s_peerlist *peerlist, struct torrent *tor, void *data);
