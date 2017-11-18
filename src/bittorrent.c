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
    s_announce *ann = tracker_announce(tor);
    if (!ann)
      printf("cannot fetch announcement\n");
    else
      bdata_print(stdout, ann->bencoded);
    announce_free(ann);
    torrent_free(tor);
  }

  return EXIT_SUCCESS;
}
