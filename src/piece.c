#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bencode.h"
#include "filelist.h"
#include "piece.h"


static void piece_init(s_piece *piece, size_t file, size_t size, char *sha)
{
  piece->file = file;
  piece->state = PIECE_MISSING;
  piece->size = size;
  piece->block_done = 0;
  strncpy(piece->sha, sha, 20);
}


s_piece *pieces_create(s_filelist *filelist, s_bdata *info,
                       size_t piece_size)
{
  s_bdata *bsha = bdict_find(info->data.dict, "pieces");
  size_t nbpieces = bsha->data.str.size / 20;
  s_piece *pieces = malloc(sizeof (s_piece) * nbpieces);
  if (!pieces)
    return NULL;
  size_t cur_piece = 0;
  size_t size = 0;
  size_t piece_file = 0;
  for (size_t i = 0; i < filelist->nbfiles; i++)
  {
    size += filelist->files[i].size;
    if (size >= piece_size)
    {
      assert(nbpieces != cur_piece);
      piece_init(pieces + cur_piece, piece_file, piece_size,
                 bsha->data.str.data + (cur_piece * 20));
      cur_piece++;
      size -= piece_size;
      piece_file = i + (!size);
    }
  }
  if (size)
    piece_init(pieces + cur_piece, piece_file, size,
                 bsha->data.str.data + (cur_piece * 20));
  return pieces;
}


bool block_write(struct torrent *tor, void *data, size_t size, size_t off)
{
  // TODO
  tor = tor;
  data = data;
  size = size;
  off = off;
  return false;
}
