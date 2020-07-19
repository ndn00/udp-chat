#ifndef _DISPLAY_H_
#define _DISPLAY_H_
#endif

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

void Display_init(const ListBuffer* pListBuffer);

void Display_exit();

void Display_signal_print();