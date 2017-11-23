#pragma once

#include "torrent.h"
#include "transmission.h"
#include "peer_conn.h"


t_trans_status receive_body(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status);


t_trans_status receive_type(struct torrent *tor, struct peer_conn *conn,
                            struct trans *trans, t_trans_status status);


t_trans_status receive_message(struct torrent *tor, struct peer_conn *conn,
                               struct trans *trans, t_trans_status status);
