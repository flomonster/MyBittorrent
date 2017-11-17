#include "buf.h"

#include <stdlib.h>


s_buf *buf_create(size_t size)
{
  s_buf *ret = malloc(sizeof(*ret) + size);
  if (!ret)
    return NULL;

  ret->size = size;
  return ret;
}
