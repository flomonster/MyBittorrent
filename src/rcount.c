#include "rcount.h"
#include <stdlib.h>
#include <assert.h>


static inline void *get_datablk(void *metadata)
{
  struct rcount_meta *pmeta = metadata;
  return pmeta + 1;
}

static inline void *get_metadata(void *data)
{
  struct rcount_meta *pmeta = data;
  return pmeta - 1;
}


void *rcount_malloc(size_t size, f_rcount_dest dest)
{
  struct rcount_meta *meta = malloc(size + sizeof(struct rcount_meta));
  if (!meta)
    return NULL;
  meta->dest = dest;
  meta->count = 1;
  return get_datablk(meta);;
}

void rcount_ref(void *data)
{
  struct rcount_meta *meta = get_metadata(data);
  meta->count += 1;
}

void rcount_deref(void *data)
{
  if (!data)
    return;

  struct rcount_meta *meta = get_metadata(data);
  assert(meta->count > 0);
  if (!(meta->count -= 1))
  {
    if (meta->dest)
      meta->dest(get_datablk(meta));
    free(meta);
  }
}
