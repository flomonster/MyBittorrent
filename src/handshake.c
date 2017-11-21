#include "log.h"
#include "transmission.h"
#include "torrent.h"
#include "peer_conn.h"
#include "decision_send.h"

#include <string.h>


void handshake_send(struct torrent *tor, struct peer_conn *conn,
                    struct trans *trans)
{
  s_bthandshake *hs = &conn->handshake_out;
  hs->plen = PROTO_NAME_LEN;
  memcpy(hs->pstr, PROTO_NAME, PROTO_NAME_LEN);
  memset(hs->reserved, 0, 8);
  memcpy(hs->info_hash, tor->metainfo.sha, 20);
  memcpy(hs->peer_id, tor->peer_id, 20);

  trans_setup(trans, decision_send, hs, sizeof(s_bthandshake));
}


static t_trans_status handshake_receive_full(struct torrent *tor,
                                             struct peer_conn *conn,
                                             struct trans *trans,
                                             t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  s_bthandshake *hs = &conn->handshake_out;
  if (hs->plen != 19)
  {
    LOG(L_NETDBG, "handshake", tor, "mismatched proto string length");
    // TODO: close the socket when a callback returns CLOSING
    return TRANS_CLOSING;
  }
  LOG(L_NETDBG, "handshake", tor, "full handshake received");
  // TODO: actual callback action
  trans_setup(trans, decision_send, &hs->pstr, sizeof(s_bthandshake) - 1);
  return TRANS_DONE;
}


void handshake_receive(struct torrent *tor, struct peer_conn *conn,
                       struct trans *trans)
{
  LOG(L_NETDBG, "handshake", tor, "reading the handshake proto string length");
  s_bthandshake *hs = &conn->handshake_in;
  // first receive the protocol string length
  trans_setup(trans, handshake_receive_full, &hs->plen, 1);
}
