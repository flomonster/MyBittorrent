#include "log.h"
#include "btproto.h"
#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"

#include <stdlib.h>


static const s_btlog_context *g_ctx = &LCTX(L_NETDBG, "receive_header");


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
              &conn->header_in.size,
              sizeof(conn->header_in.size));
  return TRANS_RETRY;
}


t_trans_status receive_type(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (!conn->header_in.size)
    return receive_message(tor, conn, trans, status);

  conn->header_in.size = ntohl(conn->header_in.size);
  btlog(g_ctx, tor, "receiving the type, message size is %lu",
        conn->header_in.size);

  trans_setup(trans, receive_body,
              &conn->header_in.type,
              sizeof(conn->header_in.type));
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


t_trans_status receive_body(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  e_bttype type = conn->header_in.type;

  btlog(g_ctx, tor, "receiving the body, type %s", bttype_to_string(type));
  conn->header_in.size--; // the type is a byte long

  trans_setup(trans, body_cleanup,
              malloc(conn->header_in.size), // TODO: actual buffering
              conn->header_in.size);
  return TRANS_RETRY;
}
