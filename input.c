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
static char* buffer = NULL;
static bool b_shutdown = false;

// static FILE* fp;

void* Input_scan(void* unused) {
  // fp = fopen("input.txt", "a+");
  buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
  while (!b_shutdown) {
    fgets(buffer, MAX_BUFFER, stdin);
    // fgets(buffer, MAX_BUFFER, fp);

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);
    b_shutdown = Shutdown_strcmp(buffer);
  }
  return NULL;
}

void Input_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Input_scan, NULL) == 0);
}

void Input_exit() {
  // fclose(fp);
  if (!b_shutdown) {
    assert(pthread_cancel(threadPID) == 0);
  }
  free(buffer);
}
void Input_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }
