#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>

#include "btopts.h"


struct btopts g_btopts;


static struct option g_long_options[] =
{
  {"verbose",                   no_argument, &g_btopts.verbose, true},
  {"pretty-print-torrent-file", no_argument, &g_btopts.btmode, METAINFO_PRINT},
  {"dump-peers",                no_argument, &g_btopts.btmode, DUMP_PEERS},
  {"seed",                      no_argument, &g_btopts.seed,   true},
  {0, 0, 0, 0}
};


int btopts_parse(int argc, char *argv[])
{
  int opt_i = 0;
  int c;

  while ((c = getopt_long(argc, argv, "", g_long_options, &opt_i)) != -1)
    switch (c)
    {
    case 0:
      break;
    default:
      /* on error or unexpected code,
      ** let the caller cleanup
      */
      return -1;
    }

  // optind is the internal getopt cursor
  return optind;
}
