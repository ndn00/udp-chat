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
    assert(pthread_mutex_lock(&mutex) == 0);
    { assert(pthread_cond_wait(&cond, &mutex) == 0); }
    assert(pthread_mutex_unlock(&mutex) == 0);

    // Critical Section
    char* buffer = (char*)ListBuffer_dequeue(plb);

    // Receive (blocking call)
    assert(sendto(socketfd, buffer, strlen(buffer), 0, remoteinfo->ai_addr,
                  remoteinfo->ai_addrlen) != -1);
    bool shutdown = Shutdown_check(buffer);
    free(buffer);
    if (shutdown) {
      Shutdown_signal();
    }
  }
  return NULL;
}
void Send_signal_transfer() {
  assert(pthread_mutex_lock(&mutex) == 0);
  { assert(pthread_cond_signal(&cond) == 0); }
  assert(pthread_mutex_unlock(&mutex) == 0);
}

void Send_init(ListBuffer* pListBuffer, const int* pSfd,
               struct addrinfo* pRinfo) {
  socketfd = *pSfd;
  remoteinfo = pRinfo;
  plb = pListBuffer;
  assert(pthread_mutex_init(&(mutex), NULL) == 0);
  assert(pthread_cond_init(&(cond), NULL) == 0);
  assert(pthread_create(&threadPID, NULL, Send_transfer, NULL) == 0);
}
void Send_exit() {
  // assert(pthread_mutex_unlock(&mutex) == 0);
  // assert(pthread_mutex_destroy(&mutex) == 0);
  // fputs("destroyed mutex", stdout);
  // fflush(stdout);
  // assert(pthread_cond_signal(&cond) == 0);
  // assert(pthread_cond_destroy(&cond) == 0);
  // fputs("destroyed cond", stdout);
  // fflush(stdout);
  assert(pthread_cancel(threadPID) == 0);
  assert(pthread_join(threadPID, NULL) == 0);
}
