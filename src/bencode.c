#include "bencode.h"

#include <stdbool.h>
#include <stdlib.h>


// dest_parse modifies its target buffer metadata
static s_bdata *bencode_dest_parse(s_dbuf *buf);


static bool parse_uint(t_bint *res, s_dbuf *buf, char stop)
{
  *res = 0;
  while (buf->size)
  {
    int cchr = buf_pop(buf);
    if (cchr == BUF_EOF)
      return true;

    if (cchr == stop)
      return false;

    if (cchr < '0' || cchr > '9')
      return true;

    *res = *res * 10 + (cchr - '0');
  }
  return true;
}


static bool parse_int(t_bint *res, s_dbuf *buf, char stop)
{
  bool sign;
  if ((sign = (buf_peek(buf) == '-')))
    buf_pop(buf);

  if (parse_uint(res, buf, stop))
    return true;

  *res *= sign ? -1 : 1;
  return false;
}



static s_bdata *bencode_int_parse(s_dbuf *buf)
{
  if (buf_pop(buf) != 'i')
    return NULL;

  t_bint i;
  if (parse_int(&i, buf, 'e'))
    return NULL;

  s_bdata *res = malloc(sizeof(*res));
  if (!res)
    return NULL;

  res->type = BINT;
  res->data.i = i;
  return res;
}


static s_bdata *bencode_str_parse(s_dbuf *buf)
{
  t_bint ssize;
  if (parse_uint(&ssize, buf, ':'))
    return NULL;

  size_t ussize = ssize;
  if (ussize > buf->size)
    return NULL;

  s_bdata *res = malloc(sizeof(*res));
  if (!res)
    return NULL;

  res->type = BSTR;
  res->data.str = S_BSTR(buf->data, ssize);

  buf_shrink(buf, ssize);
  return res;
}


static s_bdict *bdict_parse_item(s_dbuf *buf)
{
  s_bdata *key = bencode_str_parse(buf);
  if (!key)
    return NULL;

  s_bdata *value = bencode_dest_parse(buf);
  if (!value)
    return NULL;

  s_bdict *res = malloc(sizeof(*res));
  if (!res)
    return NULL;

  res->key = key;
  res->value = value;
  res->next = NULL;
  return res;
}


static s_bdata *bencode_dict_parse(s_dbuf *buf)
{
  if (buf_pop(buf) != 'd')
    return NULL;

  s_bdata *res = malloc(sizeof(*res));
  if (!res)
    return NULL;

  res->type = BDICT;
  res->data.dict = NULL;

  for (s_bdict **ip = &res->data.dict; buf_peek(buf) != BUF_EOF;)
  {
    if (buf_peek(buf) == 'e')
    {
      buf_pop(buf);
      return res;
    }

    s_bdict *dcur = bdict_parse_item(buf);
    if (!dcur)
    {
      free(res);
      return NULL;
    }
    *ip = dcur;
    ip = &dcur->next;
  }
  return NULL;
}


static s_blist_node *blist_parse_item(s_dbuf *buf)
{
  s_bdata *value = bencode_dest_parse(buf);
  if (!value)
    return NULL;

  s_blist_node *res = malloc(sizeof(*res));
  if (!res)
    return NULL;

  res->value = value;
  res->next = NULL;
  return res;
}


static s_bdata *bencode_list_parse(s_dbuf *buf)
{
  if (buf_pop(buf) != 'l')
    return NULL;

  s_bdata *res = malloc(sizeof(*res));
  if (!res)
    return NULL;

  res->type = BLIST;
  res->data.list = malloc(sizeof(s_blist));
  if (!res->data.list)
  {
    free(res);
    return NULL;
  }
  res->data.list->tail = NULL;
  res->data.list->size = 0;

  s_blist_node **ip = &res->data.list->tail;
  while (buf_peek(buf) != BUF_EOF)
  {
    if (buf_peek(buf) == 'e')
    {
      buf_pop(buf);
      return res;
    }

    s_blist_node *dcur = blist_parse_item(buf);
    if (!dcur)
    {
      free(res);
      return NULL;
    }
    *ip = dcur;
    ip = &dcur->next;
    res->data.list->size++;
  }
  return NULL;
}


static s_bdata *bencode_dest_parse(s_dbuf *buf)
{
  char *buf_beg = buf->data;
  int cchr = buf_peek(buf);
  if (cchr == BUF_EOF)
    return NULL;

  s_bdata *res;
  if (cchr == 'l')
    res = bencode_list_parse(buf);
  if (cchr == 'd')
    res = bencode_dict_parse(buf);
  if (cchr == 'i')
    res = bencode_int_parse(buf);
  if (cchr >= '0' && cchr <= '9')
    res = bencode_str_parse(buf);
  if (res)
    res->range = S_DBUF(buf_beg, buf->data - buf_beg);
  return res;
}


s_bdata *bencode_parse(const s_dbuf *buf)
{
  s_dbuf buf_cpy = *buf;
  return bencode_dest_parse(&buf_cpy);
}
