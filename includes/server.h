#pragma once

#include <stdbool.h>


typedef struct server
{
  // boilerplate
  int port;
} s_server;


bool *server_init(s_server *server);
