#include "metainfo.h"
#include "fmap.h"


bool metainfo_init(s_metainfo *mi, const char *path)
{
  if (!(mi->raw = fmap(path)))
    return true;

  if (!(mi->bencoded = bencode_parse(mi->raw)))
    return true;

  return false;
}
