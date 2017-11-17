#include <stdio.h>
#include <stdlib.h>

#include "btopts.h"
#include "fmap.h"
#include "metainfo.h"


int main(int argc, char *argv[])
{
  int torstart = btopts_parse(argc, argv);
  if (torstart < 0)
    return EXIT_FAILURE;

  for (int i = torstart; i < argc; i++)
  {
    s_metainfo mi;
    metainfo_init(&mi, argv[i]);

    bdata_print(stdout, mi.bencoded);

    metainfo_destroy(&mi);
  }

  return EXIT_SUCCESS;
}
