#include "log.h"
#include "btproto.h"
#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"
#include "receive_message.h"
#include "bitset.h"

#include "receive_have.h"

#include <stdint.h>
#include <stdlib.h>


static const s_btlog_context *g_ctx = &LCTX(L_NETDBG, "receive_have");



static inline t_trans_status receive_have_ack(struct torrent *tor,
                                              struct peer_conn *conn,
                                              struct trans *trans,
                                              t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  uint32_t piece_id = ntohl(conn->in_buf.have_id);
  if (piece_id >= tor->nbpieces)
  {
    LOG(L_ERR, "receive_have", tor, "have id out of bounds");
    return TRANS_CLOSING;
  }

  bitset_set(conn->peer->pieces, piece_id, true);
  return receive_message(tor, conn, trans, status);
}



t_trans_status receive_have(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  size_t size = conn->in_buf.header.size;
  if (size != 4)
  {
    LOG(L_ERR, "receive_have", tor, "wrong payload size, "
        "expected 4, got %zu", size);
    return TRANS_CLOSING;
  }

  btlog(g_ctx, tor, "receiving a have message");

  trans_setup(trans, receive_have_ack, &conn->in_buf.have_id, 4);
  return TRANS_RETRY;
}
