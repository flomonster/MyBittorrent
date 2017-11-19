#pragma once

#include "filelist.h"


typedef enum piece_state
{
  PIECE_MISSING,
  PIECE_FETCHING,
  PIECE_AVAILABLE,
} e_piece_state;


typedef struct piece
{
  s_filelist *filelist;
  e_piece_state state;

  // total piece size
  size_t size;

  // number of retrieved blocks
  size_t block_done;
} s_piece;
