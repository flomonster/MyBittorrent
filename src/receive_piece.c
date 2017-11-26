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
  s_btpiece *pbuf = &conn->in_buf.piece;
  if (pbuf->index == PIECE_INDEX(conn->receiving.piece, tor))
  {
    // we received all the data, exit
    conn->ask_blocks = true;
    LOG(L_LNETDBG, "receive_piece", tor, "received block %zu %zu",
        PIECE_INDEX(conn->receiving.piece, tor), conn->receiving.block);
    bitset_set(conn->blocks, conn->receiving.block, true);
  }
  return receive_message(tor, conn, trans, status);
}



static t_trans_status receive_piece_iter(s_torrent *tor, s_peer_conn *conn,
                                         s_trans *trans, t_trans_status status)
{
  if (status != TRANS_DONE)
    return status;

  s_btpiece *pbuf = &conn->in_buf.piece;
  bool intended = pbuf->index == PIECE_INDEX(conn->receiving.piece, tor);
  LOG(L_LNETDBG, "receive_piece", tor, "receiving piece part");


  if (!pbuf->header.size)
    return receive_piece_cleanup(tor, conn, trans, status);

  if (pbuf->header.size + pbuf->begin > tor->pieces[pbuf->index].size)
  {
    LOG(L_WARN, "receive_piece", tor, "received OOB piece");
    return TRANS_CLOSING; // TODO: make the caller cleanup
  }

  void *buf = NULL;
  size_t readsize = pbuf->header.size;
  if (intended)
  {
    s_piece *piece = &tor->pieces[pbuf->index];
    size_t size; // what we shall write
    buf = piece_get_buffer(tor, piece, conn->in_buf.piece.begin, &size);
    readsize = MIN(size, readsize);
  }
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
    LOG(L_WARN, "receive_piece", tor, "host sent a block he wasn't "
        "asked for: %zu, %zu", btp->index, btp->begin);

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
