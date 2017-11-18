#include <stdarg.h>
#include <stdio.h>

#include "btopts.h"
#include "torrent.h"
#include "log.h"


#define LOGLEVEL_METADATA(Name, Color)              \
  { #Name, Color },


static const struct logmeta
{
  const char *name;
  const char *color;
} g_loglevel_metadata[] =
{
  LOGLEVELS(LOGLEVEL_METADATA)
};


void btlog_init(void)
{
  g_log_config.target = stderr;
  g_log_config.level = DEFAULT_LOGLEVEL;
  g_log_config.mask = DEFAULT_LOGMASK;
}


static void print_hex_byte(void *data)
{
  unsigned char *cdata = data;
  fprintf(g_log_config.target, "%02x", *cdata);
}


static void print_torrent_sha(s_torrent *tor)
{
  for (unsigned i = 0; i < 6; i++)
    print_hex_byte(tor->metainfo.sha + i);
}


static void prelude_fancy(s_btlog_context *context, s_torrent *tor)
{
  if (tor)
  {
    fprintf(g_log_config.target, COLOR_LBLUE "[");
    print_torrent_sha(tor);
    fprintf(g_log_config.target, "]" COLOR_RESET);
  }

  const struct logmeta *lm = &g_loglevel_metadata[context->level];
  fprintf(g_log_config.target, "%s[%s][", lm->color, lm->name);
  fprintf(g_log_config.target, "%s]\t" COLOR_RESET, context->component);
}


static void prelude_boring(s_btlog_context *context, s_torrent *tor)
{
  if (tor)
  {
    print_torrent_sha(tor);
    fprintf(g_log_config.target, ": ");
  }
  fprintf(g_log_config.target, "%s:\t", context->component);
}


void btlog(s_btlog_context *context, s_torrent *tor, const char *fmt, ...)
{
  if (!((1UL << context->level) & g_log_config.mask)
      || context->level > g_log_config.level)
    return;

  va_list ap;
  va_start(ap, fmt);

  (g_log_config.fancy ? prelude_fancy : prelude_boring)(context, tor);
  vfprintf(g_log_config.target, fmt, ap);
  fputc('\n', g_log_config.target);

  va_end(ap);
}
