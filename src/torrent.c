#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "bencode.h"
#include "filelist.h"
#include "log.h"
#include "metainfo.h"
#include "peer.h"
#include "piece.h"
#include "server.h"
#include "torrent.h"
#include "tracker.h"


#define PEER_ID_BEGIN "-MB2020-"

static void peer_id_gen(t_peer_id pi)
{
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "0123456789";

  for (size_t i = 0; i < sizeof (t_peer_id); i++)
    pi[i] = charset[rand() % sizeof (charset)];
  memcpy(pi, PEER_ID_BEGIN, sizeof(PEER_ID_BEGIN) - 1);
}


static void arch_init(s_torrent *tor)
{
  s_bdata *bencoded = tor->metainfo.bencoded;
  if (filelist_init(&tor->filelist, bencoded))
    errx(2, "torrent_create: filelist_init failed");

  s_bdata *binfo = bdict_find(bencoded->data.dict, "info");
  s_bdata *bpiece_size = bdict_find(binfo->data.dict, "piece length");
  tor->piece_size = bpiece_size->data.i;
  s_bdata *bsha = bdict_find(binfo->data.dict, "pieces");
  tor->nbpieces = bsha->data.str.size / 20;
  LOG(L_DBG, "torrent", tor, "torrent has %zu pieces", tor->nbpieces);
  tor->pieces = pieces_create(&tor->filelist, binfo,
                              tor->nbpieces, tor->piece_size);
}


s_torrent *torrent_create(const char *path, bool init_arch)
{
  s_torrent *torrent = malloc(sizeof (s_torrent));
  if (!torrent)
    errx(1, "torrent_create: malloc failed");

  torrent->peerlist.peers = NULL;

  if (metainfo_init(&torrent->metainfo, path))
    errx(2, "torrent_create: metainfo_init failed");


  if (tracker_init(&torrent->tracker, &torrent->metainfo))
    errx(2, "torrent_create: tracker_init failed");

  torrent->state = T_RUNNING;

  if (server_init(&torrent->server))
    errx(2, "torrent_create: server_init failed");

  peer_id_gen(torrent->peer_id);

  if (init_arch)
    arch_init(torrent);
  else
  {
    torrent->filelist.files = NULL;
    torrent->filelist.nbfiles = 0;
    torrent->piece_size = 0;
    torrent->nbpieces = 0;
    torrent->pieces = NULL;
  }

  return torrent;
}


void torrent_free(s_torrent *tor)
{
  peerlist_destroy(&tor->peerlist);
  metainfo_destroy(&tor->metainfo);
  tracker_destroy(&tor->tracker);
  filelist_destroy(&tor->filelist);
  free(tor->pieces);
  free(tor);
}
