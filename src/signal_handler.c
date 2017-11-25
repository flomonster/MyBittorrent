#include <signal.h>
#include "signal_handler.h"


volatile bool g_running = true;


static void sighangler(int sig)
{
  (void)sig;
  g_running = false;
}


void signal_handler_register(void)
{
   signal(SIGINT, sighangler);
}
