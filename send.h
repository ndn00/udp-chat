#ifndef _SEND_H_
#define _SEND_H_

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

void Send_init(ListBuffer* pListBuffer, const int* pSfd,
               struct addrinfo* pRinfo);

void Send_exit();

void Send_signal_transfer();

#endif