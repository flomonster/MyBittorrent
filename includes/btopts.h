#pragma once


struct btopts
{
  int verbose;
  int metainfo_print;
  int dump_peers;
  int seed;
};


extern struct btopts g_btopts;
int btopts_parse(int argc, char *argv[]);
