#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

typedef struct peer
{
  struct peer *next;
  size_t pieces;
  bool interested;
  bool choked;
  bool choking;
  struct sockaddr_in addr;
} s_peer;
