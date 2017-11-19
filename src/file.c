#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"
#include "path.h"


bool file_init(s_file *file, s_path *path, size_t size, size_t offset)
{
  file->size = size;
  file->offset = offset;
  file->path = path;
  file->data = path_map(path, size, AT_FDCWD, &file->exist);
  return file->data == NULL;
}
