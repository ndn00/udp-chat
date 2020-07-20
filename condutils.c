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
void cond_timedwait(pthread_cond_t* pcond,
                    pthread_mutex_t* pmutex) {  // sleep for 100ms
  struct timespec timeToWait;
  struct timeval now;

  gettimeofday(&now, NULL);
  int msec = 100;
  // timeToWait.tv_sec = now.tv_sec + 1;
  timeToWait.tv_sec =
      now.tv_sec + (int)((now.tv_usec * 1000 + msec * 1000000) / 1000000000);
  timeToWait.tv_nsec = (now.tv_usec * 1000 + msec * 1000000) % 1000000000;
  assert(pthread_mutex_lock(pmutex) == 0);
  {
    int err = pthread_cond_timedwait(pcond, pmutex, &timeToWait);
    assert(err == ETIMEDOUT);
  }
  assert(pthread_mutex_unlock(pmutex) == 0);
}
void cond_signal(pthread_cond_t* pcond, pthread_mutex_t* pmutex) {
  assert(pthread_mutex_lock(pmutex) == 0);
  { assert(pthread_cond_signal(pcond) == 0); }
  assert(pthread_mutex_unlock(pmutex) == 0);
}