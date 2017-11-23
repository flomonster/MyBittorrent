#include "btproto.h"

#define BTTYPE_STR(N) #N,

static const char *g_bttype_tab[] =
{
  BTTYPE(BTTYPE_STR)
};

#undef BTTYPE_STR


const char *bttype_to_string(e_bttype type)
{
  return g_bttype_tab[type];
}
