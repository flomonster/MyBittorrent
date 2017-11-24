#include "bitset.h"
#include "btproto.h"
#include "log.h"
#include "peer_conn.h"
#include "receive_interested.h"
#include "receive_message.h"
#include "torrent.h"
#include "transmission.h"



static const s_btlog_context *g_ctx = &LCTX(L_NETDBG, "receive_interested");


t_trans_status receive_interested(struct torrent *tor, struct peer_conn *conn,
                                  struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  btlog(g_ctx, tor, "receiving an interested message");
  conn->state_am.interested = true;

  return receive_message(tor, conn, trans, status);
}


t_trans_status receive_not_interested(struct torrent *tor,
                                      struct peer_conn *conn,
                                      struct trans *trans,
                                      t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  btlog(g_ctx, tor, "receiving a not interested message");
  conn->state_am.interested = false;

  return receive_message(tor, conn, trans, status);
}
