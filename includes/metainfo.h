#pragma once

#include <stdbool.h>

#include "bencode.h"
#include "sha.h"
#include "dbuf.h"


typedef struct metainfo
{
  s_bdata *bencoded;
  s_dbuf *raw;
  t_sha sha;
} s_metainfo;


bool metainfo_init(s_metainfo *mi, const char *path);
void metainfo_destroy(s_metainfo *mi);
