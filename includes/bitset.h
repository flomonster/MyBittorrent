#pragma once

#include <stdbool.h>
#include <stddef.h>


typedef struct bitset
{
  size_t count;
  unsigned char data[];
} s_bitset;



s_bitset *bitset_create(size_t size);
bool bitset_update(s_bitset *bset, char *buf, size_t size);
bool bitset_get(s_bitset *bset, size_t i);
void bitset_set(s_bitset *bset, size_t i, bool val);
void bitset_free(s_bitset *bset);
