#pragma once

#include "dbuf.h"
#include "bencode.h"

void funmap(struct dbuf *file);
struct dbuf *fmap(const char *path);
