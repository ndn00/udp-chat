#include <list.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "receive.h"

int LOCAL_PORT, REMOTE_PORT;
char* REMOTE_HOST_NAME;

int main(int argc, char* argv[]) {
  // init arguments
  LOCAL_PORT = argv[1];
  REMOTE_HOST_NAME = argv[2];
  REMOTE_PORT = argv[3];

  struct addrinfo hints, *remoteinfo, *localinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(REMOTE_HOST_NAME, REMOTE_PORT, &hints, &remoteinfo) != 0) {
    // ERROR HANDLING
  }
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, LOCAL_PORT, &hints, &localinfo) != 0) {
    // ERROR HANDLING
  }

  int socketfd = socket(localinfo->ai_family, localinfo->ai_socktype, 0);
  if (bind(socketfd, localinfo->ai_addr, localinfo->ai_addrlen) != 0) {
    // ERROR HANDLING
  }

  // ready to send/recv
}