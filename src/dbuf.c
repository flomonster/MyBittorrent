#include <stdlib.h>

#include "dbuf.h"


s_dbuf *dbuf_create(size_t size)
{
  s_dbuf *ret = malloc(sizeof(*ret));
  if (!ret)
    return NULL;

  if (!((ret->data = malloc(size)) || !size))
  {
    free(ret);
    return NULL;
  }
  ret->size = size;
  return ret;
}


void dbuf_free(s_dbuf *buf)
{
  if (buf)
    free(buf->data);
  free(buf);
}


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
