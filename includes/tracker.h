#pragma once

#include "bencode.h"

typedef struct tracker
{
  /**
  ** \brief This isn't a pointer to the original entry of the
  ** metainfo structure, but rather a null-terminated copy.
  */
  char *url;
} s_tracker;


s_tracker *tracker_create(b_data *bencode);
