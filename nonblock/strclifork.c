#include "unp.h"

void str_cli(FILE *fd, int sockfd) {
  pid_t pid;
  char sendline[MAXLINE], recvline[MAXLINE];

  if ((pid_t = Fork()) == 0) {
    while (Readline(sockfd, recvline, MAXLINE) > 0) {
      Fputs(recvline, stdout);
    }
    kill(getppid(), SIGTERM);
    exit(0);
  }

  while (Fgets(sendline, MAXLINE, fp) != NULL) {
    Writen(sockfd, sendline, strlen(sendline));
  }

  Shutdown(sockfd, SHUT_WR);
  pause();
  return;
}
