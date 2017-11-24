#pragma once

#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"


t_trans_status receive_have(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status);
