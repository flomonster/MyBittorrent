#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <sys/epoll.h>


typedef struct pollfd
{
  int fd;
  bool has_data;
} s_pollfd;


typedef struct poller
{
  int fd;
  size_t max_events;
  struct epoll_event *events;
} s_poller;


bool poller_init(s_poller *pol, size_t max_events);
void poller_destroy(s_poller *pol);

bool poller_register(s_poller *pol, s_pollfd *pollfd);
bool poller_update(s_poller *pol, int timeout);
