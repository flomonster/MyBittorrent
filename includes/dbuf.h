#pragma once

#include <stddef.h>
#include <stdbool.h>


typedef struct dbuf
{
  char *data;
  size_t size;
} s_dbuf;


#define S_DBUF(Data, Size)                     \
  (s_dbuf)                                     \
  {                                            \
    .data = (Data),                            \
    .size = (Size),                            \
  }

#define BUF_EOF (-1)


void dbuf_free(s_dbuf *buf);
s_dbuf *dbuf_create(size_t size);
int buf_peek(s_dbuf *buf);
int buf_pop(s_dbuf *buf);
bool buf_shrink(s_dbuf *buf, size_t off);
