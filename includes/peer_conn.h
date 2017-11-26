#pragma once

#include "peer.h"
#include "piece.h"
#include "poller.h"
#include "btproto.h"
#include "torrent.h"
#include "transmission.h"
#include "piece_ex.h"


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
  bool pieces_changed;

  s_peer_state state_am;
  s_peer_state state_peer;

  s_piece_ex sending;
  s_piece_ex receiving;

  s_pollfd socket;

  s_trans in_trans;
  s_trans out_trans;

  // if true, decision_send will ask for new blocks
  bool ask_blocks;

  // the tick of the last send block request
  size_t last_req_tick;
  s_bitset *blocks;
  s_bitset *req_blocks;

  u_btbuf out_buf;
  u_btbuf in_buf;
} s_peer_conn;


void peer_conn_free(s_peer_conn *conn);
void peer_conn_trade(s_peer_conn *conn, s_torrent *tor);
void peer_conn_init(s_peer_conn *conn, s_torrent *tor, s_poller *pol);
void peer_conn_clear(s_peer_conn *conn, bool active);
