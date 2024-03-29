#pragma once

#include "filelist.h"
#include "sha.h"


struct torrent;


typedef enum piece_state
{
  PIECE_MISSING,
  PIECE_FETCHING,
  PIECE_AVAILABLE,
} e_piece_state;


#define PIECE_INDEX(Piece, Tor) ((Piece) - (Tor)->pieces)


typedef struct piece
{
  e_piece_state state;

  // index in the torrent filelist
  size_t file;

  // sha1 of the piece
  t_sha sha;

  // total piece size
  size_t size;
  size_t block_count;
} s_piece;


bool piece_check(s_piece *pieces, s_filelist *filelist,
                 size_t piece, size_t piece_size);
s_piece *pieces_create(s_filelist *filelist, s_bdata *info,
                       size_t nbpieces, size_t piece_size);
void *piece_get_buffer(struct torrent *tor, s_piece *piece,
                       size_t offset, size_t *size);
bool block_write(struct torrent *tor, void *data, size_t size, size_t off);
