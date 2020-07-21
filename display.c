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

static ListBuffer* plb;

void* Display_print(void* unused) {
  while (true) {
    // Critical Section
    char* buffer = (char*)ListBuffer_dequeue(plb);
    Shutdown_check(buffer);
    if (buffer != NULL) {
      fputs(buffer, stdout);
      fflush(stdout);
    }
    free(buffer);

    if (Shutdown_check(NULL)) {
      Shutdown_signal();
    }
  }
  return NULL;
}

void Display_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Display_print, NULL) == 0);
}
void Display_exit() { assert(pthread_cancel(threadPID) == 0); }

void Display_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }