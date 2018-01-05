#include "unp.h"

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen) {
  int n;
  socklen_t len;
  char mesg[MAXLINE];
  for (;;) {
    len = clilen;
    n = Recvfrom(sockfd, mesg, MAXLINE, 0, pcliaddr, &len);
    Sendto(sockfd, mesg, n, 0, pcliaddr, len);
  }
}

int main(int argc, char const *argv[]) {
  int sockfd;
  struct sockaddr_un servaddr, cliaddr;

  sockfd = Socket(AF_LOCAL, SOCK_DGRAM, 0);

  unlink(UNIXDG_PATH);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strcpy(servaddr.sun_path, UNIXDG_PATH);

  Bind(sockfd, (SA *)&servaddr, sizeof(servaddr));

  dg_echo(sockfd, (SA *)&cliaddr, sizeof(cliaddr));
 
  return 0;
}
