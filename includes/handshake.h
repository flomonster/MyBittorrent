#pragma once

#include "torrent.h"
#include "peer_conn.h"
#include "transmission.h"


void handshake_send(struct torrent *tor, struct peer_conn *conn,
                    struct trans *trans);

void handshake_receive(struct torrent *tor, struct peer_conn *conn,
                       struct trans *trans);
