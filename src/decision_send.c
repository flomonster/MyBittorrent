#include "decision.h"
#include "decision_send.h"
#include "log.h"
#include "peer_conn.h"
#include "piece_ex.h"
#include "torrent.h"
#include "transmission.h"


static t_trans_status express_interest(s_torrent *tor, s_peer_conn *conn,
                                       s_trans *trans)
{
  LOG(L_NETDBG, "decision_send", tor, "expressing our interest");
  conn->state_am.interested = true;
  conn->out_buf.header = BTSIMPLE(BTTYPE_INTERESTED);
  trans_setup(trans, decision_send, &conn->out_buf.header, sizeof(s_btheader));
  return TRANS_RETRY;
}


static t_trans_status setup_piece(s_torrent *tor, s_peer_conn *conn,
                                  s_trans *trans)
{
  if (!(conn->receiving = PIECE_EX(select_piece(conn->peer, tor))).piece)
    // we coulnd't find an interesting piece, get out
    return TRANS_RETRY;

  if (!conn->state_am.interested)
    return express_interest(tor, conn, trans);

  return TRANS_RETRY;
}


t_trans_status decision_send(s_torrent *tor, s_peer_conn *conn,
                             s_trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (tor->state == T_RUNNING && !conn->receiving.piece)
    return setup_piece(tor, conn, trans);

  return TRANS_RETRY;
}
