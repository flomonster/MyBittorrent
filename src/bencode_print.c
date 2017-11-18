#include "bencode.h"

#include <ctype.h>


static const struct
{
  char c;
  const char *eq;
} g_mapping[] =
{
  { '\\', "\\\\"},
  { '"', "\\\""},
  { '/', "\\/"},
  { '\b', "\\b"},
  { '\f', "\\f"},
  { '\n', "\\n"},
  { '\r', "\\r"},
  { '\t', "\\t"},
};


static void bdata_print_ol(FILE *f, const s_bdata *benc);


static void json_putchar(FILE *f, char c)
{
  if (!isprint(c))
  {
    unsigned char uc = c;
    fprintf(f, "\\u00%02X", uc);
  }
  else
  {
    for (size_t i = 0; i < sizeof(g_mapping) / sizeof(*g_mapping); i++)
      if (g_mapping[i].c == c)
      {
        fputs(g_mapping[i].eq, f);
        return;
      }
    fputc(c, f);
  }
}


static void bstr_print(FILE *f, const s_bdata *s)
{
  const s_bstr *str = &s->data.str;

  fputc('"', f);
  for (size_t i = 0; i < str->size; i++)
    json_putchar(f, str->data[i]);
  fputc('"', f);
}


static void bint_print(FILE *f, const s_bdata *s)
{
  fprintf(f, BINT_F, s->data.i);
}


static void blist_print(FILE *f, const s_bdata *s)
{
  fputc('[', f);
  s_blist *list = s->data.list;
  for (s_blist_node *node = list->tail; node; node = node->next)
  {
    bdata_print_ol(f, node->value);
    if (node->next)
      fputc(',', f);
  }
  fputc(']', f);
}


static void bdict_print(FILE *f, const s_bdata *s)
{
  fputc('{', f);
  for (s_bdict *dict = s->data.dict; dict; dict = dict->next)
  {
    bdata_print_ol(f, dict->key);
    fputc(':', f);
    bdata_print_ol(f, dict->value);
    if (dict->next)
      fputc(',', f);
  }
  fputc('}', f);
}


static void bdata_print_ol(FILE *f, const s_bdata *benc)
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


void bdata_print(FILE *f, const s_bdata *benc)
{
  bdata_print_ol(f, benc);
  fputc('\n', f);
}
