#pragma once

#include <stddef.h>
#include <stdint.h>
#include <arpa/inet.h>

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


#define BTHEADER(Size, Type)                    \
  (s_btheader)                                  \
  {                                             \
    .size = htonl(Size),                        \
    .type = (Type),                             \
  }


#define BTSIMPLE(Type) \
  BTHEADER(sizeof(s_btheader) - 4, Type)


typedef struct btrequest
{
  s_btheader header;
  uint32_t index;
  uint32_t begin;
  uint32_t length;
} ATTR(packed) s_btrequest;


#define BTREQUEST(Index, Begin, Length)         \
  (s_btrequest)                                 \
  {                                             \
    .header = BTHEADER(sizeof(s_btrequest) - 4, \
                       BTTYPE_REQUEST),         \
    .index = htonl(Index),                      \
    .begin = htonl(Begin),                      \
    .length = htonl(Length),                    \
  }


typedef struct btpiece
{
  s_btheader header;
  uint32_t index;
  uint32_t begin;
} ATTR(packed) s_btpiece;



#define REQUESTS_BUF_MAX 3

typedef union
{
  s_btheader header;
  s_btrequest requests[REQUESTS_BUF_MAX];
  s_bthandshake handshake;
  // the piece shares the received header
  s_btpiece piece;
  uint32_t have_id;
} u_btbuf;


const char *bttype_to_string(e_bttype type);
