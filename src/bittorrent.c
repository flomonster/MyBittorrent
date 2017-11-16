#include <stdio.h>
#include <stdlib.h>

#include "btopts.h"


int main (int argc, char **argv)
{
  int torstart = btopts_parse(argc, argv);
  if (torstart < 0)
    return EXIT_FAILURE;

  puts("ok");
  return EXIT_SUCCESS;
}
