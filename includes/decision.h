#pragma once

#include "peer.h"
#include "torrent.h"


s_piece *select_piece(s_peer *peer, s_torrent *tor);
