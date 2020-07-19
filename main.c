#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "display.h"
#include "input.h"
#include "listbuffer.h"
#include "receive.h"
#include "send.h"
#include "shutdown.h"

int main(int argc, char *argv[]) {
  struct addrinfo hints;
  struct addrinfo *remoteinfo, *localinfo;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  assert(getaddrinfo(argv[2], argv[3], &hints, &remoteinfo) == 0);
  hints.ai_flags = AI_PASSIVE;
  assert(getaddrinfo(NULL, argv[1], &hints, &localinfo) == 0);

  int socketfd = socket(PF_INET, SOCK_DGRAM, 0);
  assert(socketfd != -1);
  assert(bind(socketfd, localinfo->ai_addr, localinfo->ai_addrlen) != -1);

  ListBuffer *plb_display, *plb_send;
  plb_display = ListBuffer_init();
  plb_send = ListBuffer_init();

  Input_init(plb_send);
  Send_init(plb_send, &socketfd, remoteinfo);
  Display_init(plb_display);
  Receive_init(plb_display, &socketfd);

  // wait
  Shutdown_wait();

  Input_exit();
  Send_exit();
  Display_exit();
  Receive_exit();

  freeaddrinfo(remoteinfo);
  freeaddrinfo(localinfo);
  ListBuffer_free(plb_display);
  ListBuffer_free(plb_send);
}