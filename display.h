#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "listbuffer.h"

void Display_init(ListBuffer* pListBuffer);

void Display_exit();

void Display_waitForShutdown();

// void Display_signal();

#endif