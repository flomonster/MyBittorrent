#pragma once

#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"


t_trans_status receive_choke(struct torrent *tor, struct peer_conn *conn,
                             struct trans *trans, t_trans_status status);


t_trans_status receive_unchoke(struct torrent *tor, struct peer_conn *conn,
                               struct trans *trans, t_trans_status status);
