#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "btopts.h"
#include "announce.h"
#include "torrent.h"



int torrent_mode(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    s_torrent *tor = torrent_create(argv[i]);
    s_announce *ann = tracker_announce(tor);
    if (!ann)
      warnx("cannot fetch announcement");
    else
      bdata_print(stdout, ann->bencoded);
    announce_free(ann);
    torrent_free(tor);
  }
  return EXIT_SUCCESS;
}


int metainfo_print(int argc, char *argv[])
{
  for (int i = 0; i < argc; i++)
  {
    s_torrent *tor = torrent_create(argv[i]);
    if (!tor)
      return EXIT_FAILURE;
    bdata_print(stdout, tor->metainfo.bencoded);
    torrent_free(tor);
  }
  return EXIT_SUCCESS;
}


int main(int argc, char *argv[])
{
  int torstart = btopts_parse(argc, argv);
  if (torstart < 0)
    // exit when torstart < 0,
    // but succeed when torstart == -1
    return torstart + 1;

  argc -= torstart;
  argv += torstart;

  switch (g_btopts.btmode)
  {
  case TORRENT_MODE:
    return torrent_mode(argc, argv);
  case METAINFO_PRINT:
    return metainfo_print(argc, argv);
  case DUMP_PEERS:
    warnx("peer dumping isn't implemented for now");
    return EXIT_FAILURE;
  }
}
