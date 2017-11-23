#pragma once

#include "peer.h"
#include "piece.h"
#include "poller.h"
#include "btproto.h"
#include "torrent.h"
#include "transmission.h"


typedef struct piece_ex
{
  s_piece *piece;

  // index of the current block inside the piece
  size_t block;

  // offset inside the block
  size_t off;
} s_piece_ex;


typedef struct peer_state
{
  bool choking;
  bool interested;
} s_peer_state;


#define PEER_STATE(Choking, Interested)         \
  (s_peer_state)                                \
  {                                             \
    .choking = (Choking),                       \
    .interested = (Interested),                 \
  }


typedef struct peer_conn
{
  bool active;

  s_peer *peer;
  s_bitset *pieces;

  s_peer_state state_am;
  s_peer_state state_peer;

  s_piece_ex sending;
  s_piece_ex receiving;

  s_pollfd socket;

  s_trans in_trans;
  s_trans out_trans;

  s_btbuf in_buf;
  s_btbuf out_buf;
} s_peer_conn;


void peer_conn_trade(s_peer_conn *conn, s_torrent *tor);
void peer_conn_init(s_peer_conn *conn, s_torrent *tor, s_poller *pol);
void peer_conn_clear(s_peer_conn *conn, bool active);
