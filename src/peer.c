#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

#include "peer.h"
#include "netutils.h"


bool peer_init(s_peer *peer, void *data)
{
  peer->pieces = NULL;
  peer->addr.sin_family = AF_INET;

  uint32_t *nip = data;
  peer->addr.sin_addr.s_addr = *nip;

  char *tmp = data;
  tmp += 4;
  data = tmp;
  uint16_t *nport = data;
  peer->addr.sin_port = *nport;
  peer->conn = NULL;
  peer->fail_count = 0;
  return false;
}


static inline bool connect_wait(int sock, struct timeval tv)
{
  fd_set myset;
  FD_ZERO(&myset);
  FD_SET(sock, &myset);
  if (select(sock + 1, NULL, &myset, NULL, &tv) <= 0)
    return true;

  int valopt;
  socklen_t osize = sizeof(int);
  getsockopt(sock, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &osize);
  return !!valopt; // if an error occured, the socket isn't ready
}


int peer_connect(s_peer *peer, struct timeval tv)
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0 || fd_setblock(sock, false) < 0)
    return -1;

  void *sockaddr = &peer->addr;
  if (connect(sock, sockaddr, sizeof(peer->addr)) < 0)
    if (errno != EINPROGRESS || connect_wait(sock, tv))
    {
      close(sock);
      return -1;
    }
  fd_setblock(sock, true);
  return sock;
}
