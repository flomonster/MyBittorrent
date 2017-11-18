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

s_torrent *torrent_create(const char *path)
{
  s_torrent *torrent = malloc(sizeof (s_torrent));
  if (!torrent)
    err(1, "torrent_create: malloc failed");

  torrent->peers = NULL;

  if (metainfo_init(&torrent->metainfo, path))
    err(2, "torrent_create: metainfo_init failed");

  if (tracker_init(&torrent->tracker, &torrent->metainfo))
    err(2, "torrent_create: tracker_init failed");

  torrent->state = T_STARTING;

  if (server_init(&torrent->server))
    err(2, "torrent_create: server_init failse");

  peer_id_gen(torrent->peer_id);
  torrent->pieces = NULL;

  return torrent;
}
