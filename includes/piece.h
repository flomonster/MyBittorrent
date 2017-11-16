#pragma once

#include "filelist.h"

typedef struct piece
{
  s_filelist *filelist;
  bool state;
  size_t dirty;
  void *blkmap;
  void *buf;
} s_piece;
