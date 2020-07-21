#include "send.h"

#include <assert.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "condutils.h"
#include "listbuffer.h"
#include "shutdown.h"

#define MAX_BUFFER 4000

static pthread_t threadPID;

static int socketfd;
static struct addrinfo* remoteinfo;
static ListBuffer* plb;

void* Send_transfer(void* unused) {
  while (true) {
    // Critical Section
    char* buffer = (char*)ListBuffer_dequeue(plb);

    // Sending
    assert(sendto(socketfd, buffer, strlen(buffer), 0, remoteinfo->ai_addr,
                  remoteinfo->ai_addrlen) != -1);
    Shutdown_check(buffer);
    free(buffer);
    if (Shutdown_check(NULL)) {
      Shutdown_signal();
    }
  }
  return NULL;
}

void Send_init(ListBuffer* pListBuffer, const int* pSfd,
               struct addrinfo* pRinfo) {
  socketfd = *pSfd;
  remoteinfo = pRinfo;
  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Send_transfer, NULL) == 0);
}
void Send_exit() { assert(pthread_cancel(threadPID) == 0); }

void Send_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }