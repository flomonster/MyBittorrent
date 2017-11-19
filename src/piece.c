#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bencode.h"
#include "filelist.h"
#include "piece.h"
#include "torrent.h"

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))


struct data_loop
{
  size_t cur_piece;
  size_t size;
  size_t piece_file;
  bool exist;
};


#define DATA_LOOP(Cur_piece, Size, Piece_file, Exist)\
  (struct data_loop)                                 \
  {                                                  \
    .cur_piece = (Cur_piece),                        \
    .size = (Size),                                  \
    .piece_file = (Piece_file),                      \
    .exist = (Exist),                                \
  }


struct sha_state
{
  char *sha;
  bool exist;
};


#define SHA_STATE(Sha, Exist)                       \
  (struct sha_state)                                \
  {                                                 \
    .sha = (Sha),                                   \
    .exist = (Exist),                               \
  }


static void piece_init(s_piece *piece, size_t file, size_t size,
                      struct sha_state *sha_state)
{
  piece->file = file;
  piece->state = PIECE_MISSING;
  piece->size = size;
  piece->block_done = 0;
  strncpy(piece->sha, sha_state->sha, 20);
}


s_piece *pieces_create(s_filelist *filelist, s_bdata *info,
                       size_t nbpieces, size_t piece_size)
{
  s_bdata *bsha = bdict_find(info->data.dict, "pieces");
  s_piece *pieces = malloc(sizeof (s_piece) * nbpieces);
  if (!pieces)
    return NULL;
  struct data_loop dl = DATA_LOOP(0, 0, 0, true);
  for (size_t i = 0;
       (dl.size += filelist->files[i].size) + 1 && i < filelist->nbfiles; i++)
    if (dl.size >= piece_size)
    {
      assert(nbpieces != dl.cur_piece);
      piece_init(pieces + dl.cur_piece, dl.piece_file, piece_size,
               &SHA_STATE(bsha->data.str.data + (dl.cur_piece * 20), dl.exist));
      dl.cur_piece++;
      dl.size -= piece_size;
      dl.piece_file = i + (!dl.size);
      dl.exist = !(dl.size) || filelist->files[i].exist;
    }
    else
      dl.exist = dl.exist && filelist->files[i].exist;
  if (dl.size)
    piece_init(pieces + dl.cur_piece, dl.piece_file, dl.size,
               &SHA_STATE(bsha->data.str.data + (dl.cur_piece * 20), dl.exist));
  return pieces;
}


bool block_write(s_torrent *tor, void *data, size_t size, size_t off)
{
  s_piece *piece = &tor->pieces[off / tor->piece_size];
  char *c_data = data;

  size_t file_i = piece->file;
  size_t cur_off = off;
  size_t target_off = off + size;

  for (; cur_off < target_off; file_i++)
  {
    if (file_i >= tor->filelist.nbfiles)
      return true;
    size_t start_off = tor->filelist.files[file_i].offset;
    size_t end_off = start_off + tor->filelist.files[file_i].size;
    size_t total = MIN(end_off, target_off) - cur_off;
    char *data_file = tor->filelist.files[file_i].data;
    for (size_t i = 0; i < total; i++)
      data_file[i + cur_off - start_off] = c_data[cur_off];
    cur_off = end_off;
  }
  return false;
}
