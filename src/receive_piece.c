#include "bitset.h"
#include "block.h"
#include "btproto.h"
#include "log.h"
#include "mathutils.h"
#include "peer_conn.h"
#include "receive_message.h"
#include "torrent.h"
#include "transmission.h"

#include "receive_piece.h"

#include <stdint.h>
#include <stdlib.h>


static t_trans_status receive_piece_cleanup(s_torrent *tor,
                                            s_peer_conn *conn,
                                            s_trans *trans,
                                            t_trans_status status)
{
  // we received all the data, exit
  conn->ask_blocks = true;
  LOG(L_LNETDBG, "receive_piece", tor, "received block %zu %zu",
      PIECE_INDEX(conn->receiving.piece, tor), conn->receiving.block);
  bitset_set(conn->blocks, conn->receiving.block, true);
  return receive_message(tor, conn, trans, status);
}



static t_trans_status receive_piece_iter(s_torrent *tor, s_peer_conn *conn,
                                         s_trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  LOG(L_LNETDBG, "receive_piece", tor, "receiving piece part");

  s_btpiece *pbuf = &conn->in_buf.piece;

  if (!pbuf->header.size)
    return receive_piece_cleanup(tor, conn, trans, status);

  if (pbuf->header.size + pbuf->begin > tor->pieces[pbuf->index].size)
  {
    LOG(L_WARN, "receive_piece", tor, "received OOB piece");
    return TRANS_CLOSING; // TODO: make the caller cleanup
  }

  s_piece *piece = &tor->pieces[pbuf->index];
  size_t size; // what we shall write
  void *buf = piece_get_buffer(tor, piece, conn->in_buf.piece.begin, &size);
  size_t remaining = pbuf->header.size; // remaining data
  size_t readsize = MIN(size, remaining);
  pbuf->header.size -= readsize;
  pbuf->begin += readsize;
  trans_setup(trans, receive_piece_iter, buf, readsize);
  return TRANS_RETRY;
}


static t_trans_status receive_piece_conv(s_torrent *tor, s_peer_conn *conn,
                                         s_trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  s_btpiece *btp = &conn->in_buf.piece;
  btp->index = ntohl(btp->index);
  btp->begin = ntohl(btp->begin);

  if (btp->index != PIECE_INDEX(conn->receiving.piece, tor))
  { // TODO: might cause a race
    LOG(L_WARN, "receive_piece", tor, "host sent a block he wasn't asked for");
    return TRANS_CLOSING;
  }

  conn->receiving.block = btp->begin / MAX_BLOCK_SIZE;
  if (conn->receiving.block >= conn->receiving.piece->block_count)
  {
    LOG(L_WARN, "receive_piece", tor, "host sent an OOB block");
    return TRANS_CLOSING;
  }

  if (btlog_active(L_SNETDBG))
  {
    char *pf = peer_format(conn->peer);
    LOG(L_SNETDBG, "msg: recv", tor, "%s: piece %zu %zu",
        pf, btp->index, btp->begin);
    free(pf);
  }

  return receive_piece_iter(tor, conn, trans, status);
}



t_trans_status receive_piece(s_torrent *tor, s_peer_conn *conn,
                             s_trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  if (conn->in_buf.piece.header.size < 8)
  {
    LOG(L_WARN, "receive_piece", tor, "message size too small");
    return TRANS_CLOSING;
  }

  s_btpiece *btp = &conn->in_buf.piece;
  btp->header.size -= 8;
  trans_setup(trans, receive_piece_conv, &btp->index, 8);
  return TRANS_RETRY;
}
