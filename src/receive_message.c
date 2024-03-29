#include "log.h"
#include "btproto.h"
#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"
#include "receive_bitset.h"
#include "receive_have.h"
#include "receive_choke.h"
#include "receive_interested.h"
#include "receive_piece.h"

#include <stdlib.h>


static const s_btlog_context *g_ctx = &LCTX(L_LNETDBG, "recv_msg");


t_trans_status receive_body(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status);


t_trans_status receive_type(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status);


t_trans_status receive_message(struct torrent *tor, struct peer_conn *conn,
                               struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  btlog(g_ctx, tor, "receiving the size");

  trans_setup(trans, receive_type,
              &conn->in_buf.header.size,
              sizeof(conn->in_buf.header.size));
  return TRANS_RETRY;
}


t_trans_status receive_type(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (!conn->in_buf.header.size)
    return receive_message(tor, conn, trans, status);

  conn->in_buf.header.size = ntohl(conn->in_buf.header.size);

  btlog(g_ctx, tor, "receiving the type, message size is %lu",
        conn->in_buf.header.size);

  trans_setup(trans, receive_body,
              &conn->in_buf.header.type,
              sizeof(conn->in_buf.header.type));
  return TRANS_RETRY;
}


static inline t_trans_status body_cleanup(struct torrent *tor,
                                          struct peer_conn *conn,
                                          struct trans *trans,
                                          t_trans_status status)
{
  btlog(g_ctx, tor, "body received, freeing buffer");
  free(trans->buf);
  return receive_message(tor, conn, trans, status);
}


static const f_trans_callback g_message_handlers[] =
{
  receive_choke,
  receive_unchoke,
  receive_interested,
  receive_not_interested,
  receive_have,
  receive_bitset,
  NULL,
  receive_piece,
  NULL, // cancel
  NULL, // port
};


t_trans_status receive_body(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  e_bttype type = conn->in_buf.header.type;
  if (type >= BTTYPE_INVALID)
  {
    LOG(L_ERR, "receive_body", tor, "invalid message type %02x", type);
    return TRANS_CLOSING;
  }

  btlog(g_ctx, tor, "receiving the body, type %s", bttype_to_string(type));
  conn->in_buf.header.size--; // the type is a byte long

  if (type < BTTYPE_INVALID && g_message_handlers[type])
    return g_message_handlers[type](tor, conn, trans, status);

  LOG(L_WARN, "receive_body", tor, "unknown type %s", bttype_to_string(type));

  trans_setup(trans, body_cleanup,
              malloc(conn->in_buf.header.size), // TODO: actual buffering
              conn->in_buf.header.size);
  return TRANS_RETRY;
}
