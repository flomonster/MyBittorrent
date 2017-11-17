#include <stdlib.h>
#include <string.h>

#include "bencode.h"


static void bencode_free_dict(s_bdict *l)
{
  for (s_bdict *p = NULL; l || p; p = l, !l || (l = l->next))
  {
    free(p);
    if (!l)
      continue;
    bencode_free(l->key);
    bencode_free(l->value);
  }
}


static void bencode_free_list(s_blist *l)
{
  for (s_blist *p = NULL; l || p; p = l, !l || (l = l->next))
  {
    free(p);
    if (l)
      bencode_free(l->value);
  }
}


void bencode_free(s_bdata *benc)
{
  if (!benc)
    return;

  switch (benc->type)
  {
  case BINT: case BSTR:
    break;
  case BDICT:
    bencode_free_dict(benc->data.dict);
    break;
  case BLIST:
    bencode_free_list(benc->data.list);
    break;
  }
  free(benc);
}


s_bdata *bdict_find(s_bdict *dict, char *key)
{
  while (dict && strncmp(dict->key->data.str.data,
                         key, dict->key->data.str.size))
    dict = dict->next;

  if (dict)
    return dict->value;

  return NULL;
}
