#ifndef _RECEIVE_H_
#define _RECEIVE_H_

#include "listbuffer.h"

void Receive_init(ListBuffer* pListBuffer, const int* sfd);

void Receive_exit();

#endif