#include <stdlib.h>
#include <unistd.h>

#include "event_loop.h"
#include "log.h"


#define MAX_EVENTS 60
#define TIMEOUT 5
#define LOOP_MAX 10000


bool event_loop(s_torrent *tor)
{
  s_peer_conn *peer_conns = calloc(PEER_CONN_COUNT, sizeof(*peer_conns));
  if (!peer_conns)
    return true;

  for (size_t i = 0; i < PEER_CONN_COUNT; i++)
    peer_conn_clear(&peer_conns[i], false);

  s_poller poller;
  if (poller_init(&poller, tor, MAX_EVENTS))
    return true;

  size_t i = 0;
  do { // TODO: update the peer list if needed
    for (size_t i = 0; i < PEER_CONN_COUNT; i++)
      if (peer_conns[i].active)
        peer_conn_trade(&peer_conns[i], tor);
      else
        peer_conn_init(&peer_conns[i], tor, &poller);
    i++;
    sleep(1);
  } while (i < LOOP_MAX && !poller_update(&poller, tor, TIMEOUT));

  if (i == LOOP_MAX)
    LOG(L_WARN, "eloop", tor,
        "stopped event loop as the counter reached %zu", i);
  poller_destroy(&poller);
  free(peer_conns);
  return false;
}
