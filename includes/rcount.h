#pragma once

#include <stddef.h>


typedef void (*f_rcount_dest)(void *data);

struct rcount_meta
{
  size_t count;
  f_rcount_dest dest;
};

void *rcount_malloc(size_t size, f_rcount_dest dest);
void rcount_ref(void *data);
void rcount_deref(void *data);
