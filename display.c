#include "display.h"

#include <assert.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "listbuffer.h"
#include "shutdown.h"

#define MAX_BUFFER 100

static pthread_t threadPID;
static pthread_mutex_t mutex;
static pthread_cond_t cond;

static ListBuffer* plb;

void* Display_print(void* unused) {
  while (true) {
    assert(pthread_mutex_lock(&mutex) == 0);
    { assert(pthread_cond_wait(&cond, &mutex) == 0); }
    assert(pthread_mutex_unlock(&mutex) == 0);

    // Critical Section
    char* buffer = (char*)ListBuffer_dequeue(plb);
    bool shutdown = Shutdown_check(buffer);
    // fputs("<< ", stdout);
    fputs(buffer, stdout);
    fflush(stdout);
    free(buffer);

    if (shutdown) {
      Shutdown_signal();
    }
  }
  return NULL;
}

void Display_signal_print() {
  assert(pthread_mutex_lock(&mutex) == 0);
  { assert(pthread_cond_signal(&cond) == 0); }
  assert(pthread_mutex_unlock(&mutex) == 0);
}

void Display_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_mutex_init(&(mutex), NULL) == 0);
  assert(pthread_cond_init(&(cond), NULL) == 0);
  assert(pthread_create(&threadPID, NULL, Display_print, NULL) == 0);
}
void Display_exit() {
  assert(pthread_cancel(threadPID) == 0);
  assert(pthread_join(threadPID, NULL) == 0);
  // assert(pthread_mutex_destroy(&mutex) == 0);
  // assert(pthread_cond_destroy(&cond) == 0);
}
