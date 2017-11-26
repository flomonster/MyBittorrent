#include "log.h"
#include "peer_conn.h"
#include "transmission.h"

#include <assert.h>
#include <unistd.h>


// this is the maximum number of bytes a single session can transmit
#define TRANS_SESSION_MAX 4096


static inline t_trans_status trans_callback(struct torrent *tor,
                                            struct peer_conn *conn,
                                            s_trans *trans,
                                            t_trans_status status)
{
  if (!trans->callback)
  {
    LOG(L_DBG, "transmission", tor, "%s transmission left without"
        " callback", trans->type ? "recv" : "send");
    return TRANS_RETRY;
  }

  return trans->callback(tor, conn, trans, status);
}


t_trans_status trans_handle_error(s_peer_conn *conn, t_trans_status status)
{
  if (status <= 0 && !(status == TRANS_RETRY || status == TRANS_DONE))
  {
    close(conn->socket.fd);
    conn->socket.fd = -1;
  }
  return status;
}


t_trans_status transmit(s_trans *trans, struct peer_conn *conn,
                        struct torrent *tor)
{
  while (trans->transmitted < trans->total)
  {
    ssize_t res = trans->transmitter(&conn->socket,
                                     trans->buf + trans->transmitted,
                                     trans->total - trans->transmitted,
                                     trans->trunc * MSG_TRUNC);
    if (res <= 0)
    {
      if (res == TRANS_RETRY)
        return res;
      return trans_handle_error(conn, trans_callback(tor, conn, trans, res));
    }
    trans->transmitted += res;
  }

  assert(trans->transmitted == trans->total);
  trans->transmitted = trans->total = 0;

  return trans_handle_error(conn, trans_callback(tor, conn, trans, TRANS_DONE));
}


void trans_setup(s_trans *trans, f_trans_callback callback,
                 void *buf, size_t size)
{
  trans->trunc = !buf;
  trans->callback = callback;
  trans->buf = buf;
  trans->total = size;
  trans->transmitted = 0;
}
