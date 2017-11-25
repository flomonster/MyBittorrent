#include "decision.h"
#include "decision_send.h"
#include "log.h"
#include "peer_conn.h"
#include "piece_ex.h"
#include "torrent.h"
#include "transmission.h"
#include "block.h"

#include <assert.h>


static bool update_interest(s_torrent *tor, s_peer_conn *conn,
                            s_trans *trans, bool interest)
{
  if (interest == conn->state_am.interested)
    return false;

  LOG(L_NETDBG, "decision_send", tor, "expressing our interest");

  conn->state_am.interested = interest;
  conn->out_buf.header = BTSIMPLE(BTTYPE_INTERESTED + !interest);
  trans_setup(trans, decision_send, &conn->out_buf.header, sizeof(s_btheader));
  return true;
}


static bool setup_piece(s_torrent *tor, s_peer_conn *conn,
                        s_trans *trans)
{
  if (!(conn->receiving = PIECE_EX(select_piece(conn->peer, tor))).piece)
    // we coulnd't find an interesting piece, get out
    return false;

  update_interest(tor, conn, trans, true);
  bitset_free(conn->blocks);
  conn->blocks = bitset_create(conn->receiving.piece->block_count);
  return true;
}


static bool send_block_requests(s_torrent *tor, s_peer_conn *conn,
                                s_trans *trans)
{
  size_t piece_index = PIECE_INDEX(conn->receiving.piece, tor);
  size_t piece_size = conn->receiving.piece->size;
  size_t curblk = 0;
  for (size_t i = 0; i < conn->blocks->size && curblk < REQUESTS_BUF_MAX; i++)
    if (!bitset_get(conn->blocks, i))
    {
      conn->out_buf.requests[i] = BTREQUEST(piece_index,
                                            i * MAX_BLOCK_SIZE,
                                            BLOCK_SIZE(piece_size, i));
      curblk++;
    }

  assert(curblk > 0);
  conn->last_req_tick = tor->stats.tick;
  trans_setup(trans, decision_send, &conn->out_buf.requests,
              sizeof(s_btrequest) * curblk);
  return true;
}


t_trans_status decision_send(s_torrent *tor, s_peer_conn *conn,
                             s_trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (tor->state == T_RUNNING)
  {
    // if we were not receiving something and found something
    // to send, we may be done
    if (!conn->receiving.piece && setup_piece(tor, conn, trans))
      return TRANS_RETRY;

    // send a block request if needed
    if (conn->receiving.piece && !conn->state_am.choking
        && (conn->ask_blocks || !conn->last_req_tick
            || tor->stats.tick - conn->last_req_tick >= BLOCK_REQUEST_TIMEOUT))
      return send_block_requests(tor, conn, trans) * TRANS_RETRY;
  }

  return TRANS_RETRY;
}
