#include "unp.h"
#include "sntp.h"

int main(int argc, char const *argv[]) {
  int sockfd;
  char buf[MAXLINE];
  ssize_t n;
  socklen_t salen, len;
  struct ifi_info *ifi;
  struct sockaddr *mcastsa, *wild, *from;
  struct timeval now;
  if (arg != 2) {
    err_quit("usage: ssntp <IPaddress>");
  }
  sockfd = Udp_client(argv[1], "ntp", (void **)&mcastsa, &salen);
  wild = Malloc(salen);
  memcpy(wild, mcastsa, salen);
  sock_set_wild(wild, salen);
  Bind(sockfd, wild, salen);
#ifdef MACAST
  for (ifi = Get_ifi_info(mcastsa->sa_family, 1); ifi != NULL; ifi = ifi->ifi_next) {
    if (ifi->ifi_flags & IFF_MULTICAST) {
      Mcast_join(sockfd, mcastsa, salen, ifi->ifi_name, 0);
      printf("joined %s on %s\n", Sock_ntop(mcastsa, salen), ifi->ifi_name);
    }
  }
#endif
  from = Malloc(salen);
  for (;;) {
    len = salen;
    n = Recvfrom(sockfd, buf, sizeof(buf), 0, from, &len);
    Gettimeofday(&now, NULL);
    sntp_proc(buf, n, &now);
  }
  return 0;
}
