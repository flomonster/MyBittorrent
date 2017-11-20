#include <fcntl.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>

#include "netutils.h"


int fd_setblock(int sock, bool blocking)
{
  int oflags = fcntl(sock, F_GETFL);
  if (oflags < 0)
  {
    // TODO: perror
    close(sock);
    return oflags;
  }

  int nflags = (oflags & ~O_NONBLOCK) | O_NONBLOCK * !blocking;
  int res = fcntl(sock, F_SETFL, nflags);
  if (res < 0)
  {
    // TODO: perror
    close(sock);
    return res;
  }
  return sock;
}
