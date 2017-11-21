#pragma once

#include "torrent.h"

#include <stdbool.h>
#include <stddef.h>
#include <sys/epoll.h>


typedef struct pollfd
{
  int fd;
  bool can_recv;
  bool can_send;
} s_pollfd;


#define POLLFD(Fd)                              \
  (s_pollfd)                                    \
  {                                             \
    .fd = (Fd),                                 \
    .can_recv = true,                           \
    .can_send = true,                           \
  }


typedef struct poller
{
  int fd;
  size_t max_events;
  struct epoll_event *events;
} s_poller;


#define POLLER_EPOLL_MODE (EPOLLIN | EPOLLOUT)


struct torrent;

bool poller_init(s_poller *pol, s_torrent *tor, size_t max_events);
void poller_destroy(s_poller *pol);

bool poller_register(s_poller *pol, s_torrent *tor, s_pollfd *pollfd);
bool poller_update(s_poller *pol, s_torrent *tor, int timeout);
