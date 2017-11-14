#include <stdio.h>

#include "peers.h"

void peers_print(FILE *f, s_peers *peers)
{
  while (peers)
  {
    fprintf(f, "%s:%s\n", peers->ip, peers->port);
    peers = peers->next;
  }
}
