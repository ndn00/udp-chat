#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "display.h"
#include "input.h"
#include "listbuffer.h"
#include "receive.h"
#include "send.h"

char *LOCAL_PORT, REMOTE_PORT;
char REMOTE_HOST_NAME[20];

int main(int argc, char *argv[]) {
  // init arguments
  // LOCAL_PORT = argv[1];
  // REMOTE_HOST_NAME = argv[2];
  // REMOTE_PORT = argv[3];

  struct addrinfo hints;
  struct addrinfo *remoteinfo, *localinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  if (getaddrinfo(argv[2], argv[3], &hints, &remoteinfo) != 0) {
    // ERROR HANDLING
  }
  printf("remote addr: %d", remoteinfo->ai_addr);
  hints.ai_flags = AI_PASSIVE;
  if (getaddrinfo(NULL, argv[1], &hints, &localinfo) != 0) {
    // ERROR HANDLING
  }

  int socketfd = socket(PF_INET, SOCK_DGRAM, 0);
  if (bind(socketfd, localinfo->ai_addr, localinfo->ai_addrlen) != 0) {
    // ERROR HANDLING
  }

  ListBuffer *plb_display, *plb_send;
  plb_display = ListBuffer_init();
  plb_send = ListBuffer_init();

  printf("before making threads");
  fflush(stdout);

  Input_init(plb_send);
  Send_init(plb_send, &socketfd, remoteinfo);
  Display_init(plb_display);
  Receive_init(plb_display, &socketfd);

  printf("finished making threads");
  fflush(stdout);

  Input_exit();
  Send_exit();
  Display_exit();
  Receive_exit();
}