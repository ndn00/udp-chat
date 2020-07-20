#include "input.h"

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "listbuffer.h"
#include "send.h"
#include "shutdown.h"

#define MAX_BUFFER 100

static pthread_t threadPID;
static ListBuffer* plb;
static char* buffer;

void* Input_scan(void* unused) {
  while (true) {
    buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
    fgets(buffer, MAX_BUFFER, stdin);

    // Check eof case
    if (feof(stdin)) {
      strcpy(buffer, SHUTDOWN_STR);
    }

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);

    Send_signal_transfer();
  }
  return NULL;
}

void Input_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Input_scan, NULL) == 0);
}

void Input_exit() {
  assert(pthread_cancel(threadPID) == 0);
  assert(pthread_join(threadPID, NULL) == 0);
  free(buffer);
}
