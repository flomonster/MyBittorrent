#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "file.h"
#include "filelist.h"
#include "bencode.h"


bool filelist_init(s_filelist *filelist, s_bdata *bencode)
{
  assert(bencode->type == BDICT);
  s_bdata *data = bdict_find(bencode->data.dict, "info");
  assert(data->type == BDICT);
  s_bdata *blenght = bdict_find(data->data.dict, "length");
  if (blenght)
  {
    filelist->nbfiles = 1;
    filelist->files = malloc(sizeof (s_file));
    if (!filelist->files)
      return true;
    s_dbuf *pathname = &bdict_find(data->data.dict, "name")->data.str;
    s_path *path = path_create(pathname, NULL);
    size_t size = bdict_find(data->data.dict, "length")->data.i;
    return file_init(filelist->files, path, size, 0);
  }
  return true;
}

void filelist_destroy(s_filelist *filelist)
{
  for (size_t i = 0; i < filelist->nbfiles; i++)
  {

  }
}
