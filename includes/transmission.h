#pragma once

#include <stddef.h>

// for ssize_t
#include <sys/socket.h>

#include "poller.h"

struct peer_conn;
struct torrent;


#define TRANS_DONE (-3)
#define TRANS_RETRY (-2)
#define TRANS_ERROR (-1)
#define TRANS_CLOSING (0)

typedef ssize_t t_trans_status;


typedef enum trans_type
{
  T_TYPE_SEND,
  T_TYPE_RECV,
} e_trans_type;



// breaks the declaration dependancy between f_trans_callback
// and s_trans
struct trans;

typedef t_trans_status (*f_trans_callback)(struct torrent *tor,
                                           struct peer_conn *conn,
                                           struct trans *trans,
                                           t_trans_status status);

typedef struct trans
{
  size_t transmitted;
  size_t total;
  char *buf;

  // on -2, wait, on -1, exit, on 0, end the trans,
  // when positive, update transmitted
  ssize_t (*transmitter)(s_pollfd *state, char *buf, size_t size);

  // when all data is transmitted, call back
  f_trans_callback callback;
  e_trans_type type;
} s_trans;


#define TRANS_ACTIVE(Trans) (!!(Trans)->total)


void trans_setup(s_trans *trans, f_trans_callback callback,
                 void *buf, size_t size);

t_trans_status transmit(s_trans *trans, struct peer_conn *conn,
                        struct torrent *tor);

ssize_t pollfd_send(s_pollfd *pdf, char *buf, size_t size);
ssize_t pollfd_recv(s_pollfd *pdf, char *buf, size_t size);
