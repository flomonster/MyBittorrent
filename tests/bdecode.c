#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "bencode.h"


int main(int argc, char *argv[])
{
  if (argc < 2)
    errx(1, "missing torrent file");

  int fd = open(argv[1], O_RDONLY);
  if (fd < 0)
    err(1, "cannot open torrent");

  struct stat stat;
  if (fstat(fd, &stat) < 0)
    err(1, "cannot stat torrent");

  void *tor = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (tor == MAP_FAILED)
    err(1, "cannot map torrent file");

  s_buf buf = S_BUF(tor, stat.st_size);
  s_bdata *res = bencode_parse(&buf);
  bdata_print(stdout, res);
  puts("");
  return 0;
}
