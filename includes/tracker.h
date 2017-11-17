#pragma once

#include <stdbool.h>

#include "bencode.h"
#include "metainfo.h"


typedef struct tracker
{
  /**
  ** \brief This isn't a pointer to the original entry of the
  ** metainfo structure, but rather a null-terminated copy.
  */
  char *url;
} s_tracker;


bool tracker_init(s_tracker *tracker, s_metainfo *mi);
