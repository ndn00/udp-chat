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

static pthread_t threadPID;
static pthread_cond_t cond;
static pthread_mutex_t mutex;

static bool ShuttingDown = false;
static bool Called = false;

bool Shutdown_ConsumerReadytoShutdown(char* buffer) {
  if (Shutdown_strcmp(buffer)) {
    ShuttingDown = true;
  }
  return ShuttingDown;
}
bool Shutdown_strcmp(char* buffer) {
  return buffer != NULL ? (strcmp(buffer, SHUTDOWN_STR) == 0) : false;
}
void* Shutdown_threadKill(void* unused) {
  Input_exit();
  Receive_exit();
  Send_exit();
  Display_exit();
  return NULL;
}
void Shutdown_waitForShutdown() {
  assert(pthread_cond_init(&cond, NULL) == 0);
  assert(pthread_mutex_init(&mutex, NULL) == 0);
  Display_waitForShutdown();
  Input_waitForShutdown();
  Send_waitForShutdown();
  Receive_waitForShutdown();
  assert(pthread_join(threadPID, NULL) == 0);
  cond_destroy(&cond, &mutex);
}
void Shutdown_signal() {
  if (Called == false) {
    Called = true;
    fputs(SHUTDOWN_MSG, stdout);
    fflush(stdout);
    assert(pthread_create(&threadPID, NULL, Shutdown_threadKill, NULL) == 0);
  }
  cond_wait(&cond, &mutex);
}