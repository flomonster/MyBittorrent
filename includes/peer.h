#pragma once

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "bitset.h"


typedef char t_peer_id[20];

struct peer_conn;

typedef struct peer
{
  s_bitset *pieces;
  struct sockaddr_in addr;
  t_peer_id peer_id;
  struct peer_conn *conn;
  size_t fail_count;
} s_peer;


/**
** \param data a 6-bytes peer data block
**  as described in bep 23.
*/
bool peer_init(s_peer *peer, void *data);
int peer_connect(s_peer *peer, struct timeval tv);
