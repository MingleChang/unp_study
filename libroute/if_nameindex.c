#include "unpifi.h"
#include "unproute.h"
struct if_nameindex *if_nameindex(void) {
  char *buf, *next, *lim;
  size_t len;
  struct if_msghdr *ifm;
  struct sockaddr *sa, *rti_info[RTAX_MAX];
  struct sockaddr_dl *sdl;
  struct if_nameindex *result, *ifptr;
  char *namptr;

  if ((buf = net_rt_iflist(0, 0, &len)) == NULL) {
    return NULL;
  }

  if ((result = malloc(len)) == NULL) {
    return NULL;
  }
  ifptr = result;
  namptr = (char *)result + len;

  lim = buf + len;
  for (next = buf; next < lim, next += ifm->ifm_msglen) {
    ifm = (struct if_msghdr *)next;
    if (ifm->ifm_type == RTM_IFINFO) {
      sa = (struct sockaddr *)(ifm + 1);
      get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
      if ((sa = rti_info[RTAX_IFP]) != NULL) {
        if (sa->sa_family == AF_LINK) {
          sdl = (struct sockaddr_dl *)sa;
          namptr -= sdl->sdl_nlen +1;
          strncpy(namptr, &sdl->sdl_data[0], sdl->sdl_nlen);
          namptr[sdl->sdl_nlen] = 0;
          ifptr->if_name = namptr;
          ifptr->if_index = sdl->sdl_index;
          ifptr++;
        }
      }
    }
  }
  ifptr->if_name = NULL;
  ifptr->if_inex = 0;
  free(buf);
  return result;
}

void if_freenameindex(struct if_nameindex *ptr) {
  free(ptr);
}
