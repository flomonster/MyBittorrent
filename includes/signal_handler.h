#pragma once


#include <stdbool.h>


extern volatile bool g_running;


void signal_handler_register(void);
