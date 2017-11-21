#include "transmission.h"
#include "poller.h"

#include <errno.h>
#include <stdbool.h>
#include <sys/socket.h>


static inline bool err_would_block(int err)
{
  return err == EAGAIN || err == EWOULDBLOCK;
}


ssize_t pollfd_recv(s_pollfd *pfd, char *buf, size_t size)
{
  if (!pfd->can_recv)
    return TRANS_RETRY;

  ssize_t res = recv(pfd->fd, buf, size, 0);
  if (res < 0)
  {
    if (err_would_block(errno))
    {
      pfd->can_recv = false;
      return TRANS_RETRY;
    }
    else
      return TRANS_ERROR;
  }
  return res;
}


ssize_t pollfd_send(s_pollfd *pfd, char *buf, size_t size)
{
  if (!pfd->can_send)
    return TRANS_RETRY;

  ssize_t res = send(pfd->fd, buf, size, 0);
  if (res < 0)
  {
    if (err_would_block(errno))
    {
      pfd->can_send = false;
      return TRANS_RETRY;
    }
    else
      return TRANS_ERROR;
  }
  return res;
}
