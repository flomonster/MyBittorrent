#include "log.h"

#include <stdarg.h>
#include <stdio.h>


void btlog_prelude(e_loglevel lvl, void *custom, const char *component)
{

}


void btlog(e_loglevel lvl, void *custom, const char *component,
           const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  btlog_prelude(lvl, custom, component);
  vfprintf(g_log_target, fmt, ap);
  fputc('\n', g_log_target);

  va_end(ap);
}
