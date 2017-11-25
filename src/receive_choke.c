#include "bitset.h"
#include "btproto.h"
#include "log.h"
#include "peer_conn.h"
#include "receive_choke.h"
#include "receive_message.h"
#include "torrent.h"
#include "transmission.h"



static const s_btlog_context *g_ctx = &LCTX(L_LNETDBG, "receive_choke");


t_trans_status receive_choke(struct torrent *tor, struct peer_conn *conn,
                             struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  btlog(g_ctx, tor, "receiving a choke message");
  conn->state_am.choking = true;

  return receive_message(tor, conn, trans, status);
}


t_trans_status receive_unchoke(struct torrent *tor, struct peer_conn *conn,
                               struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  btlog(g_ctx, tor, "receiving a unchoke message");
  conn->state_am.choking = false;

  return receive_message(tor, conn, trans, status);
}
