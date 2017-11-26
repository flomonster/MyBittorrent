#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "bencode.h"
#include "block.h"
#include "filelist.h"
#include "piece.h"
#include "sha.h"
#include "torrent.h"
#include "mathutils.h"


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


static void piece_init(s_piece *piece, size_t file, size_t size, char *sha)
{
  piece->file = file;
  piece->state = PIECE_MISSING;
  piece->size = size;
  piece->block_count = REQUIRED_BLOCKS(size);
  strncpy(piece->sha, sha, 20);
}


bool piece_check(s_piece *pieces, s_filelist *filelist,
                 size_t piece, size_t piece_size)
{
  size_t off = piece * piece_size;
  size_t target = off + pieces[piece].size;
  size_t file = pieces[piece].file;
  size_t sfile = filelist->files[file].offset;
  SHA_CTX c;
  SHA1_Init(&c);

  while (sfile < target)
  {
    char *data = filelist->files[file].data;
    size_t filesize = filelist->files[file].size;
    size_t start = MAX(sfile, off);
    size_t end = MIN(target, sfile + filesize);
    SHA1_Update(&c, data + start - filelist->files[file].offset, end - start);
    sfile += filesize;
    file++;
  }

  unsigned char piece_sha[SHA_DIGEST_LENGTH];
  void *md = piece_sha;
  SHA1_Final(md, &c);
  bool ret;
  if ((ret = !memcmp(md, pieces[piece].sha, 20)))
    pieces[piece].state = PIECE_AVAILABLE;
  return ret;
}


s_piece *pieces_create(s_filelist *fl, s_bdata *info,
                       size_t nbpieces, size_t piece_size)
{
  s_bdata *bsha = bdict_find(info->data.dict, "pieces");
  s_piece *pieces = malloc(sizeof (s_piece) * nbpieces);
  if (!pieces)
    return NULL;
  struct data_loop dl = DATA_LOOP(0, 0, 0, true);
  for (size_t i = 0; i < fl->nbfiles && (dl.size += fl->files[i].size) + 1; i++)
  {
    if (!fl->files[i].size)
      continue;
    while (dl.size >= piece_size)
    {
      piece_init(pieces + dl.cur_piece, dl.piece_file, piece_size,
                 bsha->data.str.data + (dl.cur_piece * 20));
      if (dl.exist)
        piece_check(pieces, fl, dl.cur_piece, piece_size);
      dl.cur_piece++;
      dl.size -= piece_size;
      dl.piece_file = i + (!dl.size);
    }
    dl.exist = !(dl.size) || (dl.exist && fl->files[i].exist);
  }
  if (dl.size)
    piece_init(pieces + dl.cur_piece, dl.piece_file, dl.size,
               bsha->data.str.data + (dl.cur_piece * 20));
  if (dl.size && dl.exist)
    piece_check(pieces, fl, dl.cur_piece, piece_size);
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


void *piece_get_buffer(s_torrent *tor, s_piece *piece, size_t offset,
                       size_t *size)
{
  *size = piece->size - offset;
  size_t f = piece->file;
  size_t piece_offset = (piece - tor->pieces) * tor->piece_size;
  offset += piece_offset - tor->filelist.files[f].offset;
  while (!(tor->filelist.files[f].size - offset)
         || !tor->filelist.files[f].size || offset > tor->filelist.files[f].size)
  {
    offset -= tor->filelist.files[f].size;
    f++;
  }
  char *data = tor->filelist.files[f].data;
  data += offset;
  *size = MIN(tor->filelist.files[f].size - offset, *size);
  return data;
}
