#ifndef _INPUT_H_
#define _INPUT_H_
#endif

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

void Input_init(const ListBuffer* pListBuffer);

void Input_exit();