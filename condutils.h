#include <assert.h>
#include <pthread.h>

void cond_wait(pthread_cond_t* pcond, pthread_mutex_t* pmutex);
void cond_timedwait(pthread_cond_t* pcond, pthread_mutex_t* pmutex);
void cond_signal(pthread_cond_t* pcond, pthread_mutex_t* pmutex);