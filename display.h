#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "listbuffer.h"

void Display_init(ListBuffer* pListBuffer);

void Display_exit();

void Display_signal_print();

#endif