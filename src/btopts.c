#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "btopts.h"
#include "log.h"


struct btopts g_btopts;


static struct option g_long_options[] =
{
  {"fancy",                     no_argument, 0, 'f'},
  {"verbose",                   optional_argument, 0, 'v'},
  {"pretty-print-torrent-file", no_argument, &g_btopts.btmode, METAINFO_PRINT},
  {"dump-peers",                no_argument, &g_btopts.btmode, DUMP_PEERS},
  {"seed",                      no_argument, &g_btopts.seed,   true},
  {"help",                      no_argument, 0,                'h'},
  {0, 0, 0, 0}
};


static void print_help(char *pname)
{
  printf("Usage: %s [--verbose] [--seed] COMMAND\n"
         "where  COMMAND := { --dump-peers | --pretty-print-torrent-file | "
         "{ torrents } }\n",
         pname);
}


int btopts_parse(int argc, char *argv[])
{
  int opt_i = 0;
  int c;

  btlog_init();
  while ((c = getopt_long(argc, argv, "fhv::", g_long_options, &opt_i)) != -1)
    switch (c)
    {
    case 0:
      break;
    case 'v':
      g_log_config.level = optarg ? strtoul(optarg, NULL, 10) : VERBOSE_LOGLEVEL;
      break;
    case 'f':
      g_log_config.fancy = true;
      g_log_config.mask = LOGMASK_FANCY;
      break;
    case 'h':
      print_help(argv[0]);
      return -1;
    default:
      /* on error or unexpected code,
      ** let the caller cleanup
      */
      return -2;
    }

  // optind is the internal getopt cursor
  return optind;
}
