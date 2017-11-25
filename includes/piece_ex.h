#pragma once

#include "piece.h"

#include <stddef.h>


typedef struct piece_ex
{
  s_piece *piece;

  // index of the current block inside the piece
  size_t block;

  // offset inside the block
  size_t off;
} s_piece_ex;

#define PIECE_EX(Piece)                         \
  (s_piece_ex)                                  \
  {                                             \
    .piece = (Piece),                           \
    .block = 0,                                 \
    .off = 0,                                   \
  }
