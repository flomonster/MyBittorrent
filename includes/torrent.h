#pragma once

#include "peer.h"
#include "tracker.h"
#include "server.h"
#include "piece.h"
#include "metainfo.h"
#include "filelist.h"


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
