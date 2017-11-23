#include <stdlib.h>
#include <string.h>

#include "handshake.h"
#include "log.h"
#include "netutils.h"
#include "peer_conn.h"
#include "poller.h"


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
    LOG(L_NETDBG, "event_loop", tor, "closed connection");
    // TODO: close the socket ?
    conn->peer->fail_count++;
    peer_conn_clear(conn, false);
    break;
  }
}


void peer_conn_trade(s_peer_conn *conn, s_torrent *tor)
{
  handle_transmission(&conn->in_trans, conn, tor);
  handle_transmission(&conn->out_trans, conn, tor);
}


void peer_conn_init(s_peer_conn *conn, s_torrent *tor, s_poller *pol)
{
  peer_conn_clear(conn, false);
  s_peer *p = tor->peerlist.peers;
  for (; ; p = p->next)
    if (!p)
      return;
    else if (!p->conn && !p->fail_count)
      break;

  if ((conn->socket = POLLFD(peer_connect(p, CONNECT_TIMEOUT))).fd < 0)
  {
    p->fail_count++;
    return;
  }

  poller_register(pol, tor, &conn->socket);
  p->conn = conn;
  conn->peer = p;
  conn->active = true;

  if (btlog_active(L_INFO))
  {
    char *pf = peer_format(p);
    LOG(L_INFO, "peer_conn", tor, "peer connection activated -> %s", pf);
    free(pf);
  }

  handshake_send(tor, conn, &conn->out_trans);
  handshake_receive(tor, conn, &conn->in_trans);
}


void peer_conn_clear(s_peer_conn *conn, bool active)
{
  if (conn->peer)
    conn->peer->conn = NULL;
  memset(conn, 0, sizeof(*conn));
  conn->state_am = PEER_STATE(true, false);
  conn->state_peer = PEER_STATE(true, false);
  conn->active = active;
  conn->in_trans.type = T_TYPE_RECV;
  conn->in_trans.transmitter = pollfd_recv;
  conn->out_trans.type = T_TYPE_SEND;
  conn->out_trans.transmitter = pollfd_send;
}
