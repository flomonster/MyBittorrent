#pragma once

typedef struct tracker
{
  /**
  ** \brief This isn't a pointer to the original entry of the
  ** metainfo structure, but rather a null-terminated copy.
  */
  char *url;
} s_tracker;