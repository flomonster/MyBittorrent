#include <stdlib.h>
#include <unistd.h>

#include "log.h"
#include "poller.h"
#include "torrent.h"

bool poller_init(s_poller *pol, s_torrent *tor, size_t max_events)
{
  LOG(L_NETDBG, "poller", tor, "initializing");
  if (!(pol->events = malloc(sizeof(*pol->events) * max_events)))
    return true;

  if ((pol->fd = epoll_create1(0)) < 0)
  {
    free(pol->events);
    return true;
  }

  pol->max_events = max_events;
  return false;
}


void poller_destroy(s_poller *pol)
{
  free(pol->events);
}


bool poller_register(s_poller *pol, s_torrent *tor, s_pollfd *pollfd)
{
  LOG(L_NETDBG, "poller", tor, "registering fd %d", pollfd->fd);
  struct epoll_event event;
  // listen for edges of input and output events
  event.events = POLLER_EPOLL_MODE | EPOLLET;
  event.data.ptr = pollfd;
  // the poller is edge triggered, so we need to try at first
  pollfd->can_recv = true;
  pollfd->can_send = true;
  if (epoll_ctl(pol->fd, EPOLL_CTL_ADD, pollfd->fd, &event) < 0)
    // TODO: perror
    return true;
  return false;
}


static bool epoll_has_error(uint32_t event)
{
  return ((event & EPOLLERR)
         || (event & EPOLLHUP)
         || !(event & POLLER_EPOLL_MODE));
}


bool poller_update(s_poller *pol, s_torrent *tor, int timeout)
{
  int ecount = epoll_wait(pol->fd, pol->events, pol->max_events, timeout);
  if (ecount < 0)
    return true;

  for (int i = 0; i < ecount; i++)
  {
    struct epoll_event *event = &pol->events[i];
    s_pollfd *cur_pfd = event->data.ptr;
    if (epoll_has_error(event->events))
      close(cur_pfd->fd);
    else
    {
      (void)tor;
      /* if (event->events & EPOLLIN) */
      /*   LOG(L_NETDBG, "poller", tor, "input notification on fd %d", cur_pfd->fd); */
      cur_pfd->can_recv |= event->events & EPOLLIN;

      /* if (event->events & EPOLLOUT) */
      /*   LOG(L_NETDBG, "poller", tor, "output notification on fd %d", cur_pfd->fd); */
      cur_pfd->can_send |= event->events & EPOLLOUT;
    }
  }

  return false;
}
