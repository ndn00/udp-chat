#include <assert.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "display.h"
#include "input.h"
#include "listbuffer.h"
#include "receive.h"
#include "send.h"
#include "shutdown.h"

// wrapper for getaddrinfo
static void getaddr(char* node, char* serv, struct addrinfo** ppAddr) {
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  if (node == NULL) {
    hints.ai_flags = AI_PASSIVE;
  }
  assert(getaddrinfo(node, serv, &hints, ppAddr) == 0);
}

int main(int argc, char* argv[]) {
  // get local & remote info
  struct addrinfo *remoteinfo = NULL, *localinfo = NULL;
  getaddr(argv[2], argv[3], &remoteinfo);
  getaddr(NULL, argv[1], &localinfo);

  // create socket & bind to port
  int socketfd = socket(PF_INET, SOCK_DGRAM, 0);
  assert(socketfd != -1);
  assert(bind(socketfd, localinfo->ai_addr, localinfo->ai_addrlen) != -1);

  // init shared List buffer
  ListBuffer *plb_display, *plb_send;
  plb_display = ListBuffer_init();  // shared CS of display(C) & receive(P)
  plb_send = ListBuffer_init();     // shared CS of send(C) & input(P)

  // Spawning threads
  Input_init(plb_send);
  Send_init(plb_send, &socketfd, remoteinfo);
  Display_init(plb_display);
  Receive_init(plb_display, &socketfd);

  // main thread wait
  Shutdown_wait();

  // Shutting down threads
  Input_exit();
  Send_exit();
  Display_exit();
  Receive_exit();

  // Clean ups
  freeaddrinfo(remoteinfo);
  freeaddrinfo(localinfo);
  ListBuffer_free(plb_display);
  ListBuffer_free(plb_send);
  close(socketfd);
}