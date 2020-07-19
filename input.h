#ifndef _INPUT_H_
#define _INPUT_H_

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

void Input_init(ListBuffer* pListBuffer);

void Input_exit();

#endif