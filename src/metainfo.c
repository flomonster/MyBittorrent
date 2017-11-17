#include "fmap.h"
#include "metainfo.h"
#include "sha.h"


/* static bool metainfo_init_sha(s_metainfo *mi) */
/* { */
  
/* } */


bool metainfo_init(s_metainfo *mi, const char *path)
{
  if (!(mi->raw = fmap(path)))
    return true;

  if (!(mi->bencoded = bencode_parse(mi->raw)))
    return true;

  return false;
}


bool metainfo_destroy(s_metainfo *mi)
{
  bencode_free(mi->bencoded);
  funmap(mi->raw);
}
