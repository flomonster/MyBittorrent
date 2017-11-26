#include "log.h"
#include "btproto.h"
#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"
#include "receive_message.h"

#include <stdlib.h>


static const s_btlog_context *g_ctx = &LCTX(L_NETDBG, "receive_bitset");



static inline t_trans_status receive_bitset_ack(struct torrent *tor,
                                                struct peer_conn *conn,
                                                struct trans *trans,
                                                t_trans_status status)
{
  if (btlog_active(L_SNETDBG))
  {
    char *pf = peer_format(conn->peer);
    char *buf = bitset_to_string(conn->peer->pieces); // TODO: error handling
    LOG(L_SNETDBG, "msg: recv", tor, "%s: bitfield: %s", pf, buf);
    free(buf);
    free(pf);
  }

  conn->pieces_changed = true;
  return receive_message(tor, conn, trans, status);
}



t_trans_status receive_bitset(struct torrent *tor, struct peer_conn *conn,
                              struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;


  size_t bitset_size = BITSET_BSIZE(conn->peer->pieces->size);
  if (conn->in_buf.header.size > bitset_size)
  {
    LOG(L_ERR, "receive_bitset", tor, "cannot receive bitset of size %zu,"
        "buffer has size %zu", conn->in_buf.header.size, bitset_size);
    return TRANS_CLOSING; // TODO: close the connection on trans_closing
  }

  btlog(g_ctx, tor, "receiving the bitset");

  trans_setup(trans, receive_bitset_ack,
              conn->peer->pieces->data,
              conn->in_buf.header.size);
  return TRANS_RETRY;

}
