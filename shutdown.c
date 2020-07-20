#include "shutdown.h"

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define SHUTDOWN_MSG "See ya later, virgins\n"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

bool Shutdown_check(char* buffer) {
  return (strcmp(buffer, SHUTDOWN_STR) == 0);
}
void Shutdown_wait() {
  assert(pthread_mutex_lock(&mutex) == 0);
  { assert(pthread_cond_wait(&cond, &mutex) == 0); }
  assert(pthread_mutex_unlock(&mutex) == 0);
}
void Shutdown_signal() {
  fputs(SHUTDOWN_MSG, stdout);
  fflush(stdout);
  assert(pthread_mutex_lock(&mutex) == 0);
  { assert(pthread_cond_signal(&cond) == 0); }
  assert(pthread_mutex_unlock(&mutex) == 0);
  // fputs("cleaning mutexes\n", stdout);
  // fflush(stdout);
  // assert(pthread_mutex_destroy(&mutex) == 0);
  // assert(pthread_cond_destroy(&cond) == 0);
}