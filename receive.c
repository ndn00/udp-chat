#include "receive.h"

#include <netdb.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "display.h"
#include "listbuffer.h"

#define MAX_BUFFER 100

static pthread_t threadPID;
static int socketfd;
static ListBuffer* plb;
static char* buffer;

void* Receive_listen(void* unused) {
  while (true) {
    buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);

    // Receive (blocking call)
    int bytesRx = recvfrom(socketfd, buffer, MAX_BUFFER, 0,
                           (struct sockaddr*)&sinRemote, &sin_len);

    // Format into null term'd string
    int terminatedIdx = (bytesRx < MAX_BUFFER) ? bytesRx : MAX_BUFFER - 1;
    buffer[terminatedIdx] = '\0';

    // Critical Section:
    ListBuffer_enqueue(plb, buffer);

    Display_signal_print();
  }
  return NULL;
}

void Receive_init(ListBuffer* pListBuffer, const int* sfd) {
  socketfd = *sfd;
  plb = pListBuffer;
  pthread_create(&threadPID, NULL, Receive_listen, NULL);
}
void Receive_exit() {
  pthread_cancel(threadPID);
  pthread_join(threadPID, NULL);
  free(buffer);
}
