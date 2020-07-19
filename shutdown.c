#include "shutdown.h"

#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond;

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