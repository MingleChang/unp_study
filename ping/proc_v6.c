#include "ping.h"

void proc_v6(char *ptr, ssize_t len, struct msghdr *msg, struct timeval *tvrecv) {
#ifdef IPV6
  double rtt;
  struct icmp6_hdr *icmp6;
  struct timeval *tvsend;
  struct cmsghdr *cmsg;
  int hlim;
  icmp6 = (struct icmp6_hdr *)ptr;
  if (len < 0) {
    return;
  }
  if (icmp6->icmp6_type == ICMP6_ECHO_REPLY) {
    /* code */
  }
#endif
}
