#pragma once

#include "peer.h"
#include "torrent.h"


size_t select_piece(s_peer *peer, s_torrent *tor);
