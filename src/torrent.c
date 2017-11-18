#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "bencode.h"
#include "filelist.h"
#include "metainfo.h"
#include "peer.h"
#include "piece.h"
#include "server.h"
#include "torrent.h"
#include "tracker.h"

#define PEER_ID_BEGIN "-MB2020-"

static  void peer_id_gen(t_peer_id pi)
{
  const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
                         "0123456789";

  for (size_t i = 0; i < sizeof (t_peer_id); i++)
    pi[i] = charset[rand() % sizeof (charset)];
  memcpy(pi, PEER_ID_BEGIN, sizeof(PEER_ID_BEGIN) - 1);
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

  torrent->state = T_STARTING;

  if (server_init(&torrent->server))
    errx(2, "torrent_create: server_init failed");

  peer_id_gen(torrent->peer_id);
  torrent->pieces = NULL;

  if (init_arch && filelist_init(&torrent->filelist, torrent->metainfo.bencoded))
    errx(2, "torrent_create: filelist_init failed");

  return torrent;
}


void torrent_free(s_torrent *tor)
{
  metainfo_destroy(&tor->metainfo);
  tracker_destroy(&tor->tracker);
  free(tor);
}
