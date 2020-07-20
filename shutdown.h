#ifndef _SHUTDOWN_H_
#define _SHUTDOWN_H_

#include <stdbool.h>

#include "listbuffer.h"

#define SHUTDOWN_STR "!\n"

void Shutdown_wait(ListBuffer* pListBuffer1, ListBuffer* pListBuffer2);
bool Shutdown_check(char* buffer);
void Shutdown_signal();
void Shutdown_cleanup();

#endif
