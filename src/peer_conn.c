#include <string.h>

#include "log.h"
#include "peer_conn.h"


#define CONNECT_TIMEOUT                         \
  (struct timeval)                              \
  {                                             \
    .tv_sec = 1,                                \
    .tv_usec = 1,                               \
  }


static void handle_transmission(s_trans *trans, s_peer_conn *conn,
                                s_torrent *tor)
{
  if (!conn->active)
    return;

  t_trans_status st = transmit(trans, conn, tor);
  switch (st)
  {
  case TRANS_RETRY:
    break;
  case TRANS_ERROR:
  case TRANS_CLOSING:
    LOG(L_INFO, "main", tor, "closed connection");
    peer_conn_clear(conn, false);
    break;
  }
}


void peer_conn_trade(s_peer_conn *conn, s_torrent *tor)
{
  handle_transmission(&conn->in_trans, conn, tor);
  handle_transmission(&conn->out_trans, conn, tor);
}


void peer_conn_init(s_peer_conn *conn, s_torrent *tor)
{
  peer_conn_clear(conn, false);
  s_peer *p = tor->peerlist.peers;
  for (size_t i = 0; ; i++, p++)
    if (i >= tor->peerlist.nbpeers)
      return;
    else if (!p->conn && !p->fail_count)
      break;

  if ((conn->socket = POLLFD(peer_connect(p, CONNECT_TIMEOUT))).fd < 0)
  {
    p->fail_count++;
    return;
  }
  p->conn = conn;
  conn->peer = p;
  conn->active = true;
  LOG(L_INFO, "peer_conn", tor, "peer connection activated");
}


void peer_conn_clear(s_peer_conn *conn, bool active)
{
  if (conn->peer)
    conn->peer->conn = NULL;
  memset(conn, 0, sizeof(*conn));
  conn->state_am = PEER_STATE(true, false);
  conn->state_peer = PEER_STATE(true, false);
  conn->active = active;
}
