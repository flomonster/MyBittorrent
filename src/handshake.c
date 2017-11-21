#include "log.h"
#include "transmission.h"
#include "torrent.h"
#include "peer_conn.h"

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

  trans_setup(trans, NULL, hs, sizeof(s_bthandshake));
}
