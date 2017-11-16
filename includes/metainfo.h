#pragma once

#include "bencode.h"
#include "sha.h"

typedef struct metainfo
{
  s_bdata *bencoded;
  char *raw;
  t_sha sha;
} s_metainfo;
