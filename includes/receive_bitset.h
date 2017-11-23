#pragma once

#include "torrent.h"
#include "transmission.h"
#include "peer_conn.h"


t_trans_status receive_bitset(struct torrent *tor, struct peer_conn *conn,
                              struct trans *trans, t_trans_status status);
