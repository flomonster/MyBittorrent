#include "decision_send.h"
#include "log.h"
#include "transmission.h"
#include "torrent.h"
#include "peer_conn.h"


t_trans_status decision_send(struct torrent *tor, struct peer_conn *conn,
                             struct trans *trans, t_trans_status status)
{
  (void)conn;
  (void)trans;
  (void)tor;

  /* LOG(L_DBG, "decision_send", tor, "iddling in %s", */
  /*     TRANS_IS_SEND(trans) ? "send" : "recv"); */

  if (status != TRANS_DONE)
    return status;

  return TRANS_RETRY;
}
