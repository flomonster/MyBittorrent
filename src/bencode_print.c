#include "bencode.h"

#include <ctype.h>


static void bstr_print(FILE *f, const s_bdata *s)
{
  const s_bstr *str = &s->data.str;

  fputc('"', f);
  for (size_t i = 0; i < str->size; i++)
  {
    char chr = str->data[i];
    fprintf(f, isprint(chr) ? "%c" : "\\u00%02X", chr);
  }
  fputc('"', f);
}


static void bint_print(FILE *f, const s_bdata *s)
{
  fprintf(f, BINT_F, s->data.i);
}


static void blist_print(FILE *f, const s_bdata *s)
{
  fputc('[', f);
  for (s_blist *list = s->data.list; list; list = list->next)
  {
    bdata_print(f, list->value);
    if (list->next)
      fputc(',', f);
  }
  fputc(']', f);
}


static void bdict_print(FILE *f, const s_bdata *s)
{
  fputc('{', f);
  for (s_bdict *dict = s->data.dict; dict; dict = dict->next)
  {
    bdata_print(f, dict->key);
    fputc(':', f);
    bdata_print(f, dict->value);
    if (dict->next)
      fputc(',', f);
  }
  fputc('}', f);
}


void bdata_print(FILE *f, const s_bdata *benc)
{
  switch (benc->type)
  {
  case BINT:
    bint_print(f, benc);
    break;
  case BSTR:
    bstr_print(f, benc);
    break;
  case BDICT:
    bdict_print(f, benc);
    break;
  case BLIST:
    blist_print(f, benc);
    break;
  }
}
