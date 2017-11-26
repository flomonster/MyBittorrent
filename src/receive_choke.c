#include "bitset.h"
#include "btproto.h"
#include "log.h"
#include "peer_conn.h"
#include "receive_choke.h"
#include "receive_message.h"
#include "torrent.h"
#include "transmission.h"

#include <stdlib.h>


static const s_btlog_context *g_ctx = &LCTX(L_SNETDBG, "msg: recv");


t_trans_status receive_choke(struct torrent *tor, struct peer_conn *conn,
                             struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (btlog_active(L_SNETDBG))
  {
    char *pf = peer_format(conn->peer);
    btlog(g_ctx, tor, "%s: choke", pf);
    free(pf);
  }

  conn->state_am.choking = true;

  return receive_message(tor, conn, trans, status);
}


t_trans_status receive_unchoke(struct torrent *tor, struct peer_conn *conn,
                               struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (btlog_active(L_SNETDBG))
  {
    char *pf = peer_format(conn->peer);
    btlog(g_ctx, tor, "%s: unchoke", pf);
    free(pf);
  }

  conn->state_am.choking = false;

  return receive_message(tor, conn, trans, status);
}
