#pragma once


typedef enum btmode
{
  TORRENT_MODE,
  METAINFO_PRINT,
  DUMP_PEERS,
} e_btmode;


struct btopts
{
  int verbose;
  int seed;
  int btmode;
};


extern struct btopts g_btopts;
int btopts_parse(int argc, char *argv[]);
