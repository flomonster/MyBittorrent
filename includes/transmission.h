#pragma once

#include <stddef.h>

// for ssize_t
#include <sys/socket.h>


struct peer_conn;
struct torrent;


typedef enum trans_status
{
  TRANS_RETRY = -2,
  TRANS_ERROR = -1,
  TRANS_END = 0,
} e_trans_status;


typedef struct trans
{
  size_t transmitted;
  size_t total;
  char *buf;

  // on -2, wait, on -1, exit, on 0, end the trans,
  // when positive, update transmitted
  ssize_t (*transmitter)(void *state, char *buf, size_t size);

  // when all data is transmitted, call back
  e_trans_status (*callback)(struct torrent *tor, struct peer_conn *conn,
                             struct trans *trans, e_trans_status status);
} s_trans;


#define TRANS_ACTIVE(Trans) (!!(Trans)->total)


e_trans_status transmit(s_trans *trans);
