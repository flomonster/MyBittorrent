#include <stdio.h>
#include <stdlib.h>

#include "btopts.h"
#include "announce.h"
#include "torrent.h"


int main(int argc, char *argv[])
{
  int torstart = btopts_parse(argc, argv);
  if (torstart < 0)
    return EXIT_FAILURE;

  for (int i = torstart; i < argc; i++)
  {
    s_torrent *tor = torrent_create(argv[i]);
    printf("%d\n", tracker_announce(tor));
    //bdata_print(stdout, tor->metainfo.bencoded);
  }

  return EXIT_SUCCESS;
}
