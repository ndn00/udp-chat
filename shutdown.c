#include "shutdown.h"

#include <pthread.h>

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void Shutdown_wait() {
  pthread_mutex_lock(&mutex);
  { pthread_cond_wait(&cond, &mutex); }
  pthread_mutex_unlock(&mutex);
}
void Shutdown_signal() {
  pthread_mutex_lock(&mutex);
  { pthread_cond_signal(&cond); }
  pthread_mutex_unlock(&mutex);
}