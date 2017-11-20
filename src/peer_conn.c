#include <string.h>

#include "peer_conn.h"
#include <stdio.h> // TODO: remove


#define CONNECT_TIMEOUT                         \
  (struct timeval)                              \
  {                                             \
    .tv_sec = 1,                                \
    .tv_usec = 1,                               \
  }

void peer_conn_trade(s_peer_conn *conn, s_torrent *tor)
{
  (void)conn;
  (void)tor;
}


void peer_conn_init(s_peer_conn *conn, s_torrent *tor)
{
  peer_conn_clear(conn, false);
  s_peer *p = tor->peerlist.peers;
  for (size_t i = 0; ; i++)
    if (i >= tor->peerlist.nbpeers)
      return;
    else if (!p->conn)
      break;

  if ((conn->socket = POLLFD(peer_connect(p, CONNECT_TIMEOUT))).fd < 0)
    return;
  printf("A\n");
}


void peer_conn_clear(s_peer_conn *conn, bool active)
{
  memset(conn, 0, sizeof(*conn));
  conn->state_am = PEER_STATE(true, false);
  conn->state_peer = PEER_STATE(true, false);
  conn->active = active;
}
