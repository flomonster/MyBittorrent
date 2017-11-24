#pragma once

#include "peer_conn.h"
#include "torrent.h"
#include "transmission.h"


t_trans_status receive_interested(struct torrent *tor, struct peer_conn *conn,
                                  struct trans *trans, t_trans_status status);


t_trans_status receive_not_interested(struct torrent *tor,
                                      struct peer_conn *conn,
                                      struct trans *trans,
                                      t_trans_status status);
