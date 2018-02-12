#include "icmpd.h"

int readable_listen(void) {
  int i, connfd;
  socklen_t clilen;
  clilen = sizeof(cliaddr);
  connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);
  for (i = 0; i < FD_SETSIZE; i++) {
    if (client[i].connfd < 0) {
      client[i].connfd = connfd;
      break;
    }
  }
  if (i == FD_SETSIZE) {
    close(connfd);
    return --nready;
  }
  printf("new connection, i = %d, connfd = %d\n", i, connfd);
  FD_SET(connfd, &allset);
  if (connfd > maxfd) {
    maxfd = connfd;
  }
  if (i > maxi) {
    maxi = i;
  }
  return --nready;
}
