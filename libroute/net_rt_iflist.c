#include "unproute.h"

char *net_rt_iflist(int family, int flags, size_t *lenp) {
  int mib[6];
  char *buf;

  mib[0] = CTL_NET;
  mib[1] = AF_ROUTE;
  mib[2] = 0;
  mib[3] = family;
  mib[4] = NET_RT_IFLIST;
  mib[5] = flags;
  if (sysctl(mib, 6, NULL, lenp, NULL, 0) < 0) {
    return NULL;
  }

  if ((buf = malloc(*lenp)) == NULL) {
    return NULL;
  }
  if (sysctl(mib, 6, buf, lenp, NULL, 0) < 0) {
    free(buf);
    return NULL;
  }

  return buf;
}
