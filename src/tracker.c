#include <err.h>
#include <string.h>
#include <stdlib.h>

#include "tracker.h"
#include "bencode.h"
#include "metainfo.h"


bool tracker_init(s_tracker *tracker, s_metainfo *mi)
{
  s_bdict *root_dict = mi->bencoded->data.dict;
  s_dbuf url = bdict_find(root_dict, "announce")->data.str;
  if (!(tracker->url = strndup(url.data, url.size)))
    return true;
  return false;
}


void tracker_destroy(s_tracker *tracker)
{
  free(tracker->url);
}
