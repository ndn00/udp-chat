#include "condutils.h"

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>

void cond_wait(pthread_cond_t* pcond, pthread_mutex_t* pmutex) {
  assert(pthread_mutex_lock(pmutex) == 0);
  { assert(pthread_cond_wait(pcond, pmutex) == 0); }
  assert(pthread_mutex_unlock(pmutex) == 0);
}

void cond_signal(pthread_cond_t* pcond, pthread_mutex_t* pmutex) {
  assert(pthread_mutex_lock(pmutex) == 0);
  { assert(pthread_cond_signal(pcond) == 0); }
  assert(pthread_mutex_unlock(pmutex) == 0);
}

void cond_destroy(pthread_cond_t* pcond, pthread_mutex_t* pmutex) {
  assert(pthread_mutex_unlock(pmutex) == 0);
  assert(pthread_mutex_destroy(pmutex) == 0);
  assert(pthread_cond_destroy(pcond) == 0);
}