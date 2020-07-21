#include "receive.h"

#include <assert.h>
#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "condutils.h"
#include "display.h"
#include "listbuffer.h"
#include "shutdown.h"

#define MAX_BUFFER 100

static pthread_t threadPID;

static int socketfd;
static ListBuffer* plb;
static char* buffer;

void* Receive_listen(void* unused) {
  while (true) {
    buffer = (char*)malloc(MAX_BUFFER * sizeof(char));

    // dummy
    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);

    // Receive (blocking call)
    int bytesRx = recvfrom(socketfd, buffer, MAX_BUFFER, 0,
                           (struct sockaddr*)&sinRemote, &sin_len);
    assert(bytesRx != -1);
    // Format into null term'd string
    int terminatedIdx = (bytesRx < MAX_BUFFER) ? bytesRx : MAX_BUFFER - 1;
    buffer[terminatedIdx] = '\0';

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);
  }
  return NULL;
}

void Receive_init(ListBuffer* pListBuffer, const int* sfd) {
  socketfd = *sfd;
  plb = pListBuffer;
  assert(pthread_create(&threadPID, NULL, Receive_listen, NULL) == 0);
}
void Receive_exit() {
  assert(pthread_cancel(threadPID) == 0);
  free(buffer);
}

void Receive_waitForShutdown() { assert(pthread_join(threadPID, NULL) == 0); }
