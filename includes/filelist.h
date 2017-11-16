#pragma once

#include "file.h"

typedef struct filelist
{
  s_file *file;
  struct filelist *next;
} s_filelist;
