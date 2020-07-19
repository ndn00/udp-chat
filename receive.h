#ifndef _RECEIVE_H_
#define _RECEIVE_H_

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

void Receive_init(ListBuffer* pListBuffer, const int* sfd);

void Receive_exit();

#endif