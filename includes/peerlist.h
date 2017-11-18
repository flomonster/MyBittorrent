#pragma once

#include "peer.h"


struct announce;


typedef struct peerlist
{
  s_peer *peers;
  size_t nbpeers;
} s_peerlist;


bool peerlist_init(s_peerlist *peerlist, struct announce *announce);
void peerlist_print(FILE *f, s_peerlist *peerlist);
