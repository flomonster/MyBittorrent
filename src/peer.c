#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>

#include "peer.h"


bool peer_init(s_peer *peer, void *data)
{
  peer->pieces = NULL;
  peer->addr.sin_family = AF_INET;

  uint32_t *nip = data;
  peer->addr.sin_addr.s_addr = ntohl(*nip);

  char *tmp = data;
  tmp += 4;
  data = tmp;
  uint16_t *nport = data;
  peer->addr.sin_port = ntohs(*nport);

  return false;
}
