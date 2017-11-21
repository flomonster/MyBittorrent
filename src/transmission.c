#include "peer_conn.h"
#include "transmission.h"

#include <assert.h>


// this is the maximum number of bytes a single session can transmit
#define TRANS_SESSION_MAX 4096


t_trans_status transmit(s_trans *trans, struct peer_conn *conn,
                        struct torrent *tor)
{
  while (trans->transmitted < trans->total)
  {
    ssize_t res = trans->transmitter(&conn->socket,
                                     trans->buf + trans->transmitted,
                                     trans->total - trans->transmitted);
    if (res <= 0)
    {
      if (res == TRANS_RETRY)
        return res;
      else
        return trans->callback(tor, conn, trans, res);
    }
    trans->transmitted += res;
  }

  assert(trans->callback);
  assert(trans->transmitted == trans->total);
  trans->transmitted = trans->total = 0;

  return trans->callback(tor, conn, trans, TRANS_DONE);
}
