#pragma once

#include <stddef.h>
#include <stdbool.h>


typedef struct dbuf
{
  char *data;
  size_t size;
} s_dbuf;


#define S_BUF(Data, Size)                      \
  (s_dbuf)                                      \
  {                                            \
    .data = (Data),                            \
    .size = (Size),                            \
  }

#define BUF_EOF (-1)

int buf_peek(s_dbuf *buf);
int buf_pop(s_dbuf *buf);
bool buf_shrink(s_dbuf *buf, size_t off);