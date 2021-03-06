#include "display.h"

#include <assert.h>
#include <errno.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "condutils.h"
#include "listbuffer.h"
#include "shutdown.h"

#define MAX_BUFFER 4000

static pthread_t threadPID;
static char* buffer = NULL;
static bool b_shutdown = false;

static ListBuffer* plb;

void* Display_print(void* unused) {
  while (!b_shutdown) {
    fflush(stdout);
    // Critical Section
    buffer = (char*)ListBuffer_dequeue(plb);
    if (buffer != NULL) {
      fputs(buffer, stdout);
      fflush(stdout);
    }
    b_shutdown = Shutdown_ConsumerReadytoShutdown(buffer);
    free(buffer);
    buffer = NULL;

    if (b_shutdown) {
      Shutdown_signal();
    }
  }
  return NULL;
}

void Display_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Display_print, NULL) == 0);
}
void Display_exit() {
  if (!b_shutdown) {
    assert(pthread_cancel(threadPID) == 0);
  }
  free(buffer);
}

void Display_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }