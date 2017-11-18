#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#include "path.h"
#include "dbuf.h"


s_path *path_create(s_dbuf *name, s_path *next)
{
  s_path *path = malloc(sizeof (s_path));
  if (!path)
    return NULL;
  if (!(path->name = strndup(name->data, name->size)))
  {
    free(path);
    return NULL;
  }
  path->next = next;
  return path;
}


void path_free(s_path *path)
{
  while (path)
  {
    s_path *tmp = path;
    path = path->next;
    free(tmp->name);
    free(tmp);
  }
}


void *path_map(s_path *path, size_t size, int dir_fd)
{
  if (!path->next)
  {
    int fd = openat(dir_fd, path->name, O_CREAT | O_RDWR);
    if (fd < 0)
      return NULL;
    void *res = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_PRIVATE, fd, 0);
    return res == MAP_FAILED ? NULL : res;
  }
  int next_fd = openat(dir_fd, path->name, O_DIRECTORY);
  if (next_fd < 0)
    return NULL;
  void *res = path_map(path->next, size, next_fd);
  if (close(next_fd) < 0)
    return NULL;
  return res;
}
