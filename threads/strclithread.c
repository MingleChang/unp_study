#include "unpthread.h"

void *copyto(void *);

static int sockfd;
static FILE *fp;

void str_cli(FILE *fp_arg, int sockfd_arg) {
  char recvline[MAXLINE];
  pthread_t tid;
  sockfd = sockfd_arg;
  fp = fp_arg;
  Pthread_create(&tid, NULL, copyto, NULL);
  while (Readline(sockfd, recvline, MAXLINE) > 0) {
    Fputs(recvline, stdout);
  }
}

void copyto(void *arg) {
  char sendline[MAXLINE];
  while (Fgets(sendline, MAXLINE, fp) != NULL) {
    Writen(sockfd, sendline, strlen(sendline));
  }
  Shutdown(sockfd, SHUT_WR);
  return NULL;
}
