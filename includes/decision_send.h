#pragma once

#include "transmission.h"

t_trans_status decision_send(struct torrent *tor, struct peer_conn *conn,
                             struct trans *trans, t_trans_status status);
