#pragma once


#include "buf.h"

#include <stdio.h>


typedef long t_bint;
#define BINT_F "%ld"

typedef s_buf s_bstr;
#define S_BSTR(Data, Size) S_BUF(Data, Size)


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
} s_bdata;


s_bdata *bencode_parse(s_buf *buf);
void bdata_print(FILE *f, const s_bdata *benc);
