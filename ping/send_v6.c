#include "ping.h"

void send_v6(void) {
#ifdef IPV6
  int len;
  struct icmp6_hdr *icmp6;
  icmp6 = (struct icmp6_hdr *)sendbuf;
  icmp6->icmp6_type = ICMP6_ECHO_REQUEST;
  icmp6->icmp6_code = 0;
  icmp6->icmp6_id = pid;
  icmp6->icmp6_seq = nsent++;
#endif
}
