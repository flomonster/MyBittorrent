#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "announce.h"
#include "bencode.h"
#include "peer.h"
#include "peerlist.h"


bool peerlist_init(s_peerlist *peerlist, s_announce *announce)
{
  assert(announce->bencoded->type == BDICT);
  s_bdata *bpeers = bdict_find(announce->bencoded->data.dict, "peers");
  assert(bpeers->type == BSTR);

  peerlist->nbpeers = bpeers->data.str.size / 6;
  peerlist->peers = malloc(sizeof (s_peer) * peerlist->nbpeers);
  if (!peerlist->peers)
    return true;

  for (size_t i = 0; i < peerlist->nbpeers; i++)
    if (peer_init(peerlist->peers + i, bpeers->data.str.data + i * 6))
      return true;
  return false;
}


void peerlist_print(FILE *f, s_peerlist *peerlist)
{
  for (size_t i = 0; i < peerlist->nbpeers; i++)
  {
    char buffer[20];
    inet_ntop(AF_INET, &(peerlist->peers[i].addr.sin_addr), buffer, 20);
    fprintf(f, "%s:%d\n", buffer, peerlist->peers[i].addr.sin_port);
  }
}
