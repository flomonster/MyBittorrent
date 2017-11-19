#include <assert.h>
#include <err.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>


#include "file.h"
#include "filelist.h"
#include "bencode.h"

static bool singlefile_init(s_filelist *filelist, s_bdata *info)
{
  filelist->nbfiles = 1;
  filelist->files = malloc(sizeof (s_file));
  if (!filelist->files)
    return true;
  s_dbuf *pathname = &bdict_find(info->data.dict, "name")->data.str;
  s_path *path = path_create(pathname, NULL);
  size_t size = bdict_find(info->data.dict, "length")->data.i;
  return file_init(filelist->files, path, size, 0);
}


static bool multifile_init(s_filelist *filelist, s_bdata *bfiles)
{
  filelist->nbfiles = bfiles->data.list->size;
  filelist->files = malloc(sizeof (s_file) * filelist->nbfiles);
  if (!filelist->files)
    return true;

  s_blist_node *bfile = bfiles->data.list->tail;
  for (size_t i = 0; i < filelist->nbfiles; i++)
  {
    size_t size = bdict_find(bfile->value->data.dict, "length")->data.i;
    s_blist *bpath = (bdict_find(bfile->value->data.dict, "path"))->data.list;
    s_blist_node *bdir = bpath->tail;
    s_path *path = path_create(&bdir->value->data.str, NULL);
    s_path *curpath = path;
    bdir = bdir->next;
    while (bdir)
    {
      curpath->next = path_create(&bdir->value->data.str, NULL);
      curpath = curpath->next;
      bdir = bdir->next;
    }
    if (file_init(filelist->files + i, path, size, 0))
      return true;
    bfile = bfile->next;
  }
  return false;
}


bool filelist_init(s_filelist *filelist, s_bdata *bencode)
{
  assert(bencode->type == BDICT);
  s_bdata *data = bdict_find(bencode->data.dict, "info");
  assert(data->type == BDICT);
  s_bdata *blenght = bdict_find(data->data.dict, "length");
  if (blenght)
    return singlefile_init(filelist, data);

  s_bdata *bfiles = bdict_find(data->data.dict, "files");
  assert(bfiles->type == BLIST);
  return multifile_init(filelist, bfiles);
}

void filelist_destroy(s_filelist *filelist)
{
  for (size_t i = 0; i < filelist->nbfiles; i++)
  {

  }
}
