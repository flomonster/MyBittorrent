#include "dbuf.h"


bool buf_shrink(s_dbuf *buf, size_t off)
{
  if (off > buf->size)
    return true;

  buf->data += off;
  buf->size -= off;
  return false;
}


int buf_peek(s_dbuf *buf)
{
  return buf->size ? buf->data[0] : BUF_EOF;
}


int buf_pop(s_dbuf *buf)
{
  int res = buf_peek(buf);
  if (res == BUF_EOF)
    return BUF_EOF;

  buf_shrink(buf, 1);
  return res;
}
