#include "shutdown.h"

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "condutils.h"
#include "display.h"
#include "input.h"
#include "listbuffer.h"
#include "receive.h"
#include "send.h"

#define SHUTDOWN_MSG "See ya later, virgins\n"

static ListBuffer *plb1 = NULL, *plb2 = NULL;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;

static bool ShuttingDown = false;

bool Shutdown_check(char* buffer) {
  if (buffer != NULL ? (strcmp(buffer, SHUTDOWN_STR) == 0) : false) {
    ShuttingDown = true;
  }
  return ShuttingDown;
}
void Shutdown_wait(ListBuffer* pListBuffer1, ListBuffer* pListBuffer2) {
  plb1 = pListBuffer1;
  plb2 = pListBuffer2;
  cond_wait(&cond, &mutex);
}
void Shutdown_signal() {
  ListBuffer_signal(plb1);
  ListBuffer_signal(plb2);
  fputs(SHUTDOWN_MSG, stdout);
  fflush(stdout);
  cond_signal(&cond, &mutex);
  // fputs("cleaning mutexes\n", stdout);
  // fflush(stdout);
}
void Shutdown_cleanup() {
  // assert(pthread_mutex_unlock(&mutex) == 0);
  // assert(pthread_mutex_destroy(&mutex) == 0);
  // assert(pthread_cond_destroy(&cond) == 0);
}