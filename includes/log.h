#pragma once

#include <stdbool.h>
#include <stdio.h>


struct torrent;


#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_LBLUE   "\x1b[94m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_GRAY    "\x1b[90m"
#define COLOR_RESET   "\x1b[0m"


// S stands for specified.
#define LOGLEVELS(F)       \
  F(ERR,    COLOR_RED)     \
  F(WARN,   COLOR_BLUE)    \
  F(INFO,   COLOR_YELLOW)  \
  F(SINFO,  COLOR_YELLOW)  \
  F(DBG,    COLOR_MAGENTA) \
  F(NETDBG, COLOR_CYAN)    \
  F(SNETDBG, COLOR_CYAN)


#define LOGLEVEL_ENUM(Name, Color)  \
  L_ ## Name,


typedef enum loglevel
{
  LOGLEVELS(LOGLEVEL_ENUM)
} e_loglevel;


#define VERBOSE_LOGLEVEL L_SNETDBG
#define DEFAULT_LOGLEVEL L_SINFO
#define DEFAULT_LOGMASK  (1UL << L_SINFO | 1UL << L_SNETDBG)
#define LOGMASK_FANCY  (~0UL)


typedef struct logconfig
{
  bool fancy;
  e_loglevel level;
  size_t mask;
  FILE *target;
} s_logconfig;


typedef struct btlog_context
{
  e_loglevel level;
  const char *component;
} s_btlog_context;


#define LCTX(Level, Component)                  \
  (s_btlog_context)                             \
  {                                             \
    .level = (Level),                           \
    .component = (Component),                   \
  }

s_logconfig g_log_config;


void btlog_init(void);
void btlog(s_btlog_context *context, struct torrent *tor, const char *fmt, ...);

#define LOG(Level, Component, Torrent, ...)     \
  btlog(&LCTX((Level), (Component)), (Torrent), __VA_ARGS__)

#define SLOG(Level, Component, ...)    \
  LOG(Level, Component, NULL, __VA_ARGS__)
