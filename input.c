#include "input.h"

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "condutils.h"
#include "listbuffer.h"
#include "send.h"
#include "shutdown.h"

#define MAX_BUFFER 100

static pthread_cond_t cond;
static pthread_mutex_t mutex;

static pthread_t threadPID;
static ListBuffer* plb;
static char* buffer;

extern FILE* fp;

// void Input_signal() { cond_signal(&cond, &mutex); }

void* Input_scan(void* unused) {
  cond_timedwait(&cond, &mutex);

  while (true) {
    buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
    fgets(buffer, MAX_BUFFER, fp);

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);
    if (Shutdown_check(NULL)) {
      cond_wait(&cond, &mutex);
    }
  }
  return NULL;
}

void Input_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_mutex_init(&(mutex), NULL) == 0);
  assert(pthread_cond_init(&(cond), NULL) == 0);
  assert(pthread_create(&threadPID, NULL, Input_scan, NULL) == 0);
}

void Input_exit() {
  assert(pthread_cancel(threadPID) == 0);
  assert(pthread_join(threadPID, NULL) == 0);
  assert(pthread_mutex_unlock(&(mutex)) == 0);
  assert(pthread_mutex_destroy(&(mutex)) == 0);
  assert(pthread_cond_destroy(&(cond)) == 0);
  free(buffer);
}
