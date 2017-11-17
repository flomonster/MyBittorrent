#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "fmap.h"
#include "bencode.h"


static struct dbuf *fmap_fd(int fd)
{
  struct dbuf *ret = malloc(sizeof(*ret));
  if (!ret)
    return NULL;

  struct stat stat;
  if (fstat(fd, &stat) < 0)
  {
    warn("cannot stat torrent");
    return NULL;
  }

  ret->size = stat.st_size;
  if ((ret->data = mmap(NULL, ret->size, PROT_READ, MAP_PRIVATE, fd, 0))
      == MAP_FAILED)
  {
    warn("cannot map torrent file");
    return NULL;
  }

  return ret;
}


struct dbuf *fmap(const char *path)
{
  int fd = open(path, O_RDONLY);
  if (fd < 0)
  {
    warn("cannot open torrent");
    return NULL;
  }

  struct dbuf *tor = fmap_fd(fd);

  if (close(fd) < 0)
  {
    warn("cannot close torrent fd");
    funmap(tor);
    return NULL;
  }

  return tor;
}


void funmap(struct dbuf *file)
{
  if (munmap(file->data, file->size) < 0)
    warn("cannot unmap file");
  free(file);
}
