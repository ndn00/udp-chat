#include "receive.h"

#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "listbuffer.h"

#define MAX_BUFFER 40

static pthread_t threadPID;
static int socketfd;

int Receive_listen() {
  while (true) {
    char* buffer = (char*)malloc(MAX_BUFFER * sizeof(char));
    struct sockaddr_in sinRemote;
    unsigned int sin_len = sizeof(sinRemote);

    // Receive (blocking call)
    int bytesRx = recvfrom(sockfd, buffer, MAX_BUFFER, MSG_WAITALL,
                           (struct sockaddr*)&sinRemote, &sin_len);

    // Format into null term'd string
    int terminatedIdx = (bytesRx < MAX_BUFFER) ? bytesRx : MAX_BUFFER - 1;
    buffer[terminatedIdx] = '\0';

    // Critical Section:
    { Display_enqueue(buffer); }
  }
}

int Receive_init(const int* sfd) {
  socketfd = *sfd;
  int iret1 = pthread_create(&threadPID, NULL, Receive_listen, NULL);
}