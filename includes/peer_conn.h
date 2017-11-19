#pragma once

#include "peer.h"
#include "piece.h"


typedef struct piece_ex
{
  s_piece *piece;

  // index of the current block inside the piece
  size_t block;

  // offset inside the block
  size_t off;
} s_piece_ex;


typedef struct peer_conn
{
  s_peer *peer;
  bool active;
  int socket;

  bool am_choking;
  bool am_interested;
  bool peer_choking;
  bool peer_interested;

  bool handshake_pending;

  s_piece_ex sending;
  s_piece_ex receiving;

  bool can_recv;

  s_bitset *pieces;
} s_peer_conn;
