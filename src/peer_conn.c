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

    if (btlog_active(L_SNETDBG))
    {
      char *pf = peer_format(conn->peer);
      LOG(L_SNETDBG, "peers", tor, "disconnect: %s", pf);
      free(pf);
    }

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

  if (btlog_active(L_SINFO))
  {
    char *pf = peer_format(p);
    LOG(L_SINFO, "peers", tor, "connect: %s", pf);
    free(pf);
  }

  handshake_send(tor, conn, &conn->out_trans);
  handshake_receive(tor, conn, &conn->in_trans);
}


void peer_conn_free(s_peer_conn *conn)
{
  bitset_free(conn->blocks);
  bitset_free(conn->req_blocks);
}


void peer_conn_clear(s_peer_conn *conn, bool active)
{
  if (conn->receiving.piece)
    conn->receiving.piece->state = PIECE_MISSING;
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
