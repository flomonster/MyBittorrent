#pragma once

#include <stddef.h>
#include <stdint.h>

#include "attr.h"
#include "sha.h"
#include "peer.h"

typedef struct bthandshake
{
  struct
  {
    unsigned char len;
    unsigned char data[256];
  } handshake_pstr;
  unsigned char reserved[8];
  t_sha info_hash;
  t_peer_id peer_id;
} ATTR(packed) s_bthandshake;


typedef struct btheader
{
  uint32_t size;
  unsigned char type;
} ATTR(packed) s_btheader;


typedef struct btbuf
{
  union
  {
    s_btheader header;
    s_bthandshake handshake;
  } data;
} s_btbuf;
