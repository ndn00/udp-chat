#ifndef _SHUTDOWN_H_
#define _SHUTDOWN_H_

#include <stdbool.h>

#include "listbuffer.h"

#define SHUTDOWN_STR "!\n"

void Shutdown_waitForShutdown();
bool Shutdown_ConsumerReadytoShutdown(char* buffer);
bool Shutdown_strcmp(char* buffer);
void Shutdown_signal();
void Shutdown_inputWait();

#endif
