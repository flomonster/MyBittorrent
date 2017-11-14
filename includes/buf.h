#pragma once

#include <stddef.h>
#include <stdbool.h>


typedef struct buf
{
  char *data;
  size_t size;
} s_buf;


#define S_BUF(Data, Size)                      \
  (s_buf)                                      \
  {                                            \
    .data = (Data),                            \
    .size = (Size),                            \
  }

#define BUF_EOF (-1)

int buf_peek(s_buf *buf);
int buf_pop(s_buf *buf);
bool buf_shrink(s_buf *buf, size_t off);
