#pragma once

#include "bencode.h"
#include "file.h"

typedef struct filelist
{
  size_t nbfiles;
  s_file *files;
} s_filelist;


bool filelist_init(s_filelist *filelist, s_bdata *data);
void filelist_destroy(s_filelist *filelist);
