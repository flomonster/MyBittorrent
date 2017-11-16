#pragma once

typedef struct file
{
  char *path;
  void *addr;
  size_t size;
  size_t offset;
} s_file;
