#pragma once

#include <stddef.h>
#include <stdint.h>

#include "attr.h"
#include "sha.h"
#include "peer.h"


#define PROTO_NAME "BitTorrent protocol"
#define PROTO_NAME_LEN (sizeof(PROTO_NAME) - 1)


#define BTTYPE(F)     \
  F(CHOKE)            \
  F(UNCHOKE)          \
  F(INTERESTED)       \
  F(NOT_INTERESTED)   \
  F(HAVE)             \
  F(BITFIELD)         \
  F(REQUEST)          \
  F(PIECE)            \
  F(CANCEL)           \
  F(PORT)             \
  F(INVALID)


#define BTTYPE_ENUM(N) BTTYPE_ ## N,

typedef enum bttype
{
  BTTYPE(BTTYPE_ENUM)
} e_bttype;

#undef BTTYPE_ENUM


typedef struct bthandshake
{
  unsigned char plen;
  unsigned char pstr[PROTO_NAME_LEN];
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


const char *bttype_to_string(e_bttype type);
