#pragma once

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <sys/socket.h>

typedef char t_peer_id[20];

typedef struct peer
{
  size_t pieces;
  bool interested;
  bool choked;
  bool choking;
  struct sockaddr_in addr;
  t_peer_id peer_id;
} s_peer;


bool peer_init(s_peer *peer, void *data);
