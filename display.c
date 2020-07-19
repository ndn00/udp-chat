#include "display.h"

#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

#define MAX_BUFFER 100

static pthread_t threadPID;
static pthread_mutex_t mutex;
static pthread_cond_t cond;

static ListBuffer* plb;

void* Display_print(void* unused) {
  while (true) {
    pthread_mutex_lock(&mutex);
    { pthread_cond_wait(&cond, &mutex); }
    pthread_mutex_unlock(&mutex);

    // Critical Section
    char* buffer = (char*)ListBuffer_dequeue(plb);
    fputs(buffer, stdout);
    fflush(stdout);
    free(buffer);
  }
  return NULL;
}

void Display_signal_print() {
  pthread_mutex_lock(&mutex);
  { pthread_cond_signal(&cond); }
  pthread_mutex_unlock(&mutex);
}

void Display_init(ListBuffer* pListBuffer) {
  plb = pListBuffer;
  if (pthread_mutex_init(&(mutex), NULL) != 0) {
    // Error handling
  }
  if (pthread_cond_init(&(cond), NULL) != 0) {
    // Error handling
  }
  pthread_create(&threadPID, NULL, Display_print, NULL);
}
void Display_exit() { pthread_join(threadPID, NULL); }
