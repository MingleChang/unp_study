#include "trace.h"

const char *icmpcode_v6(int code) {
#ifdef IPV6
  static char errbuf[100];
  switch (code) {
    case ICMP6ICMP6_DST_UNREACH_NOPORT:{
      return "no route to host";
    }break;
    case ICMP6ICMP6_DST_UNREACH_ADMIN:{
      return "administratively prohibited";
    }break;
    case ICMP6_DST_UNREACH_NOTNEIGHBOR:{
      return "not a neighbor";
    }break;
    case ICMP6_DST_UNREACH_ADDR:{
      return "address unreachable";
    }break;
    case ICMP6_DST_UNREACH_NOPORT:{
      return "port unreachable";
    }break;
    default:{
      sprintf(errbuf, "[unknown code %d]", code);
      return;
    }
  }
#endif
}
