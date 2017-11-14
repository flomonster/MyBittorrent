#pragma once

#include <stdio.h>

#define PEER_ID_LENGTH 23
#define PEER_IP_LENGTH 256
#define PEER_PORT_LENGTH 11


typedef struct peers
{
  char id[PEER_ID_LENGTH];
  char ip[PEER_IP_LENGTH];
  char port[PEER_PORT_LENGTH];
  int available_picies;
  struct peers *next;
} s_peers;


#define S_PEERS(Id, Ip, Port, AP)             \
  (s_peers)                                   \
  {                                           \
    .id = (Id),                               \
    .ip = (Ip),                               \
    .port = (Port),                           \
    .available_picies = (AP),                 \
  }


void peers_print(FILE *f, s_peers *peers);
