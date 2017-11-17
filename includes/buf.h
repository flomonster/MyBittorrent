#pragma once

#include "dbuf.h"

#include <stddef.h>


typedef struct buf
{
  size_t size;
  char data[];
} s_buf;


#define BUF_DEMOTE(Buf) DBUF((Buf)->data, (Buf)->size)


s_buf *buf_create(size_t size);
