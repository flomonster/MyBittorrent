#include "fmap.h"
#include "metainfo.h"
#include "sha.h"


static bool metainfo_init_sha(s_metainfo *mi)
{
  s_bdata *info = bdict_find(mi->bencoded->data.dict, "info");
  if (!info)
    return true;

  void *data = info->range.data;
  void *res = mi->sha;
  SHA1(data, info->range.size, res);

  return false;
}


bool metainfo_init(s_metainfo *mi, const char *path)
{
  if (!(mi->raw = fmap(path)))
    return true;

  if (!(mi->bencoded = bencode_parse(mi->raw)))
    return true;

  return metainfo_init_sha(mi);
}


void metainfo_destroy(s_metainfo *mi)
{
  bencode_free(mi->bencoded);
  funmap(mi->raw);
}
