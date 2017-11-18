#pragma once

#include "bencode.h"
#include "filelist.h"
#include "metainfo.h"
#include "peer.h"
#include "piece.h"
#include "server.h"
#include "tracker.h"

typedef char t_peer_id[20];

typedef enum torrent_state
{
  T_STARTING,
  T_RUNNING,
  T_COMPLETE,
} e_torrent_state;


typedef struct torrent
{
  s_peer *peers;
  s_tracker tracker;
  s_metainfo metainfo;
  e_torrent_state state;
  s_server server;
  t_peer_id peer_id;
  s_piece *pieces;
} s_torrent;

s_torrent *torrent_create(const char *path);
void torrent_free(s_torrent *tor);
