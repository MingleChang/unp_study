#include "unpifi.h"

struct ifi_info *get_ifi_info(int family, int doaliases) {
  struct ifi_info *ifi, *ifihead, **ifipnext;
  int sockfd, len, lastlen, flags, myflags, idx = 0, hlen = 0;
  char *ptr, *buf, lastname[IPNAMSIZ], *cptr, *haddr, *sdlname;
}
