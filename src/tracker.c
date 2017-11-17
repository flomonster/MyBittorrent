#include <err.h>
#include <string.h>
#include <stdlib.h>

#include "tracker.h"
#include "bencode.h"


s_tracker *tracker_create(b_data *bencode)
{
  s_tracker *tracker = malloc(sizeof (s_tracker));
  if (!tracker)
    err(1, "tracker_create: malloc failed");

  s_dbuf url = bdict_find(bencode, "announce")->data.str;

  tracker->url = malloc(url.size + 1);
  if (!tracker->url)
    err(1, "tracker_create: malloc failed");

  strcpy(tracker->url, url.data);

  return tracker;
}
