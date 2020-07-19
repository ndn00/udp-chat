#include "input.h"

#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"
#include "send.h"

#define MAX_BUFFER 100

static pthread_t threadPID;
static ListBuffer* plb;
static char* buffer;
void* Input_scan(void* unused) {
  while (true) {
    buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
    fgets(buffer, MAX_BUFFER, stdin);

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);

    Send_signal_transfer();
  }
  return NULL;
}

void Input_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  pthread_create(&threadPID, NULL, Input_scan, NULL);
}

void Input_exit() {
  pthread_cancel(threadPID);
  pthread_join(threadPID, NULL);
  free(buffer);
}
