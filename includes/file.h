#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "path.h"


typedef struct file
{
  s_path *path;
  void *data;
  size_t size;
  size_t offset;
  bool exist;
} s_file;


bool file_init(s_file *file, s_path *path, size_t size, size_t offset);
void file_destroy(s_file *file);
