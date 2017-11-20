#pragma once

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
    .can_recv = false,                          \
    .can_send = false,                          \
  }


typedef struct poller
{
  int fd;
  size_t max_events;
  struct epoll_event *events;
} s_poller;


#define POLLER_EPOLL_MODE (EPOLLIN | EPOLLOUT)


bool poller_init(s_poller *pol, size_t max_events);
void poller_destroy(s_poller *pol);

bool poller_register(s_poller *pol, s_pollfd *pollfd);
bool poller_update(s_poller *pol, int timeout);
