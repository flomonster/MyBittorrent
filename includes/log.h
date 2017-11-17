#pragma once

#include <stdio.h>


typedef enum loglevel
{
  INFO,
  WARNING,
  CRITICAL,
} e_loglevel;


FILE *g_log_target;

void btlog(e_loglevel lvl, void *custom, const char *component,
           const char *fmt, ...);
