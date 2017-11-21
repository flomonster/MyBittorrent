#pragma once

#include "dbuf.h"


typedef struct path
{
  char *name;
  struct path *next;
} s_path;


s_path *path_create(s_dbuf *name, s_path *next);
void *path_map(s_path *path, size_t size, int dir_fd, bool *exist);
void path_free(s_path *path);
