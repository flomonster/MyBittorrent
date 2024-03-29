#include <errno.h>
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


void *path_map(s_path *path, size_t size, int dir_fd, bool *exist)
{
  if (!path->next)
  {
    int fd = openat(dir_fd, path->name, O_CREAT | O_EXCL | O_RDWR, 0644);

    *exist = false;
    if (fd < 0)
    {
      if (errno != EEXIST)
        return NULL;
      *exist = true;
      if ((fd = openat(dir_fd, path->name, O_CREAT | O_RDWR, 0644)) < 0)
        return NULL;
    }
    if (fd < 0)
      return NULL;

    if (fallocate(fd, 0, 0, size) < 0)
      if (errno != EOPNOTSUPP || ftruncate(fd, size) < 0)
      {
        close(fd);
        return NULL;
      }

    void *res = mmap(NULL, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    return res == MAP_FAILED ? NULL : res;
  }

  mkdirat(dir_fd, path->name, 0750);
  int next_fd = openat(dir_fd, path->name, O_DIRECTORY, 0750);
  if (next_fd < 0)
    return NULL;

  void *res = path_map(path->next, size, next_fd, exist);

  if (close(next_fd) < 0)
    return NULL;
  return res;
}
