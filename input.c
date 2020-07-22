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

#define MAX_BUFFER 4000

static pthread_t threadPID;

static ListBuffer* plb;
static pthread_cond_t cond;
static pthread_mutex_t mutex;
static char* buffer = NULL;
static bool eofFlag = false;

// static FILE* fp;

void* Input_scan(void* unused) {
  // fp = fopen("input.txt", "a+");
  buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
  while (true) {
    fgets(buffer, MAX_BUFFER, stdin);
    // fgets(buffer, MAX_BUFFER, fp);

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);
    if (Shutdown_strcmp(buffer)) {
      cond_wait(&cond, &mutex);
    }
  }
  return NULL;
}

void Input_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_cond_init(&cond, NULL) == 0);
  assert(pthread_mutex_init(&mutex, NULL) == 0);
  assert(pthread_create(&threadPID, NULL, Input_scan, NULL) == 0);
}

void Input_exit() {
  // fclose(fp);
  assert(pthread_cancel(threadPID) == 0);
  cond_destroy(&cond, &mutex);
  free(buffer);
}
void Input_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }
