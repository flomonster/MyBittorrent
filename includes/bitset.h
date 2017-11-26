#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>


typedef struct bitset
{
  size_t size;
  unsigned char data[];
} s_bitset;




#define BITSET_BSIZE(Size) (((Size) + (CHAR_BIT - 1)) / CHAR_BIT)


s_bitset *bitset_create(size_t size);
bool bitset_update(s_bitset *bset, char *buf, size_t size);
bool bitset_get(s_bitset *bset, size_t i);
void bitset_set(s_bitset *bset, size_t i, bool val);
void bitset_free(s_bitset *bset);
char *bitset_to_string(s_bitset *bitset);
void bitset_clear(s_bitset *bitset);
