#include "send.h"

#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"
#include "shutdown.h"

#define MAX_BUFFER 100

static pthread_t threadPID;
static pthread_mutex_t mutex;
static pthread_cond_t cond;

static int socketfd;
static struct addrinfo* remoteinfo;
static ListBuffer* plb;

void* Send_transfer(void* unused) {
  while (true) {
    pthread_mutex_lock(&mutex);
    { pthread_cond_wait(&cond, &mutex); }
    pthread_mutex_unlock(&mutex);

    // Critical Section
    char* buffer = (char*)ListBuffer_dequeue(plb);

    // Receive (blocking call)
    sendto(socketfd, buffer, strlen(buffer), 0, remoteinfo->ai_addr,
           remoteinfo->ai_addrlen);
    bool shutdown = (strcmp(buffer, SHUTDOWN_STR) == 0);
    free(buffer);
    if (shutdown) {
      fputs(SHUTDOWN_MSG, stdout);
      fflush(stdout);
      Shutdown_signal();
    }
  }
  return NULL;
}
void Send_signal_transfer() {
  pthread_mutex_lock(&mutex);
  { pthread_cond_signal(&cond); }
  pthread_mutex_unlock(&mutex);
}

void Send_init(ListBuffer* pListBuffer, const int* pSfd,
               struct addrinfo* pRinfo) {
  socketfd = *pSfd;
  remoteinfo = pRinfo;
  plb = pListBuffer;
  if (pthread_mutex_init(&(mutex), NULL) != 0) {
    // Error handling
  }
  if (pthread_cond_init(&(cond), NULL) != 0) {
    // Error handling
  }
  pthread_create(&threadPID, NULL, Send_transfer, NULL);
}
void Send_exit() {
  pthread_cancel(threadPID);
  pthread_join(threadPID, NULL);
}
