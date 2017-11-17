#pragma once


#include "dbuf.h"

#include <stdio.h>


typedef long t_bint;
#define BINT_F "%ld"

typedef s_dbuf s_bstr;
#define S_BSTR(Data, Size) S_DBUF(Data, Size)


typedef struct bdict
{
  struct bdata *key;
  struct bdata *value;
  struct bdict *next;
} s_bdict;


typedef struct blist
{
  struct bdata *value;
  struct blist *next;
} s_blist;


typedef struct bdata
{
  enum btype
  {
    BINT,
    BSTR,
    BDICT,
    BLIST,
  } type;

  union
  {
    t_bint i;
    s_bstr str;
    s_bdict *dict;
    s_blist *list;
  } data;
  s_dbuf range;
} s_bdata;


s_bdata *bencode_parse(const s_dbuf *buf);
void bencode_free(s_bdata *benc);
void bdata_print(FILE *f, const s_bdata *benc);
s_bdata *bdict_find(s_bdict *dict, char *key);
