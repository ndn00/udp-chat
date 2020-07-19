#ifndef _SHUTDOWN_H_
#define _SHUTDOWN_H_

#include <stdbool.h>

#define SHUTDOWN_STR "!\n"

void Shutdown_wait();
bool Shutdown_check(char* buffer);
void Shutdown_signal();

#endif
