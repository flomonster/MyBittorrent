#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "announce.h"
#include "bencode.h"
#include "peer.h"
#include "peerlist.h"
#include "rcount.h"


bool peerlist_init(s_peerlist *peerlist, s_announce *announce)
{
  assert(announce->bencoded->type == BDICT);
  s_bdata *bpeers = bdict_find(announce->bencoded->data.dict, "peers");
  assert(bpeers->type == BSTR);

  size_t nbpeers = bpeers->data.str.size / 6;
  peerlist->peers = NULL;

  for (size_t i = 0; i < nbpeers; i++)
    if (peerlist_append(peerlist, bpeers->data.str.data + i * 6))
      return true;
  return false;
}


bool peerlist_append(s_peerlist *peerlist, void *data)
{
  s_peer *peer = peer_create(&peerlist->peers, peerlist->peers, data);
  if (!peer)
    return true;
  peerlist->nbpeers++;
  if (peerlist->peers)
    peerlist->peers->prec = &peer->next;
  peerlist->peers = peer;
  return false;
}


void peerlist_print(FILE *f, s_peerlist *peerlist)
{
  s_peer *peer = peerlist->peers;
  while (peer)
  {
    char *buf = peer_format(peer);
    fprintf(f, "%s\n", buf);
    free(buf);
    peer = peer->next;
  }
}

void peerlist_destroy(s_peerlist *peerlist)
{
  s_peer *peer = peerlist->peers;
  while (peer)
  {
    s_peer *next = peer->next;
    rcount_deref(peer);
    peer = next;
  }
}
