#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen) {
  int n ;
  char sendline[MAXLINE], recvline[MAXLINE + 1];
  while (Fgets(sendline, MAXLINE, fp) != NULL) {
    Sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
    n = Recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
    recvline[n] = 0;
    Fputs(recvline, stdout);
  }
}

int main(int argc, char const *argv[]) {
  int sockfd;
  struct sockaddr_un cliaddr, servaddr;
  sockfd = Socket(AF_LOCAL, SOCK_DGRAM, 0);

  bzero(&cliaddr, sizeof(cliaddr));
  cliaddr.sun_family = AF_LOCAL;
  strcpy(cliaddr.sun_path, tmpnam(NULL));

  Bind(sockfd, (SA *)&cliaddr, sizeof(cliaddr));

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sun_family = AF_LOCAL;
  strcpy(servaddr.sun_path, UNIXDG_PATH);

  dg_cli(stdin, sockfd, (SA *)&servaddr, sizeof(servaddr));

  return 0;
}
