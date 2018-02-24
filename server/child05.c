#include "unp.h"
#include "child.h"

pid_t child_make(int i, int listenfd, int addrlen) {
  int sockfd[2];
  pid_t pid;
  void child_main(int, int, int);

  Socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfd);
  if ((pid = Fork()) > 0) {
    Close(sockfd[1]);
    cptr[i].child_pid = pid;
    cptr[i].child_pipefd = sockfd[0];
    cptr[i].child_status = 0;
    return pid;
  }
  Dup2(sockfd[1], STDERR_FILENO);
  Close(sockfd[0]);
  Close(sockfd[1]);
  Close(listenfd);
  child_main(i, listen, addrlen);
}
