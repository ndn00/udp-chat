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

// Blocking input once SHUTDOWN_STR is received
static pthread_cond_t cond;
static pthread_mutex_t mutex;
static bool deleteBuffer = true;

static ListBuffer* plb;
static char* buffer = NULL;

FILE* fp;

void* Input_scan(void* unused) {
  while (true) {
    buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
    fgets(buffer, MAX_BUFFER, fp);
    // fgets(buffer, MAX_BUFFER, stdin);

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);
    if (Shutdown_check(buffer)) {
      deleteBuffer = false;
      cond_wait(&cond, &mutex);
    }
  }
  return NULL;
}

void Input_init(ListBuffer* pListBuffer) {
  fp = fopen("input.txt", "a+");

  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Input_scan, NULL) == 0);
}

void Input_exit() {
  fclose(fp);
  assert(pthread_cancel(threadPID) == 0);
  cond_destroy(&cond, &mutex);
  if (deleteBuffer) {
    free(buffer);
  }
}
void Input_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }
