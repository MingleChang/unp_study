#include "unp.h"
#include <sys/utsname.h>

#define SENDRATE 5

void send_all(int sockfd, SA *sadest, socklen_t salen) {
  char line[MAXLINE];
  struct utsname myname;
  if (uname(&myname) < 0) {
    err_sys("uname error");
  }
  snprintf(line, sizeof(line), "%s, %d\n", myname.nodename, getpid());
  for (;;) {
    Sendto(sendfd, line, strlen(line), 0, sadest, salen);
    sleep(SENDRATE);
  }
}
