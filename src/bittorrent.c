#include <stdio.h>
#include <stdlib.h>

#include "btopts.h"
#include "fmap.h"


int main(int argc, char *argv[])
{
  int torstart = btopts_parse(argc, argv);
  if (torstart < 0)
    return EXIT_FAILURE;

  for (int i = torstart; i < argc; i++)
  {
    struct dbuf *cur_tor = fmap(argv[i]);
    s_bdata *benc_tor = bencode_parse(cur_tor);

    bdata_print(stdout, benc_tor);

    bencode_free(benc_tor);
    funmap(cur_tor);
  }

  return EXIT_SUCCESS;
}
