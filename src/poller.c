#include <stdlib.h>
#include <unistd.h>

#include "log.h"
#include "poller.h"


bool poller_init(s_poller *pol, size_t max_events)
{
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


bool poller_register(s_poller *pol, s_pollfd *pollfd)
{
  struct epoll_event event;
  // listen for edges of input and output events
  event.events = POLLER_EPOLL_MODE | EPOLLET;
  event.data.ptr = pollfd;
  pollfd->can_recv = false;
  pollfd->can_send = false;
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


bool poller_update(s_poller *pol, int timeout)
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
      cur_pfd->can_recv = event->events & EPOLLIN;
      cur_pfd->can_send = event->events & EPOLLOUT;
    }
  }

  return false;
}
