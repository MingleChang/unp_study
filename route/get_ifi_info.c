#include "unpifi.h"
#include "unproute.h"

struct ifi_info *get_ifi_info(int family, int dealiases) {
  int flags;
  char *buf, *next, *lim;
  size_t len;
  struct if_msghdr *ifm;
  struct ifa_msghdr *ifam;
  struct sockaddr *sa, *rti_info[RTAX_MAX];
  struct sockaddr_dl *sdl;
  struct ifi_info *ifi, *ifisave, *ifihead, **ifipnext;

  buf = Net_rt_iflist(family, 0, &len);

  ifihead = NULL;
  ifipnext = &ifihead;

  lim = buffer + len;
  for (next = buf; next < lim; next += ifm->ifm_msglen) {
    ifm = (struct if_msghdr *)next;
    if (ifm->ifm_type == RTM_IFINFO) {
      if (((flags = ifm->ifm_flags) & IFF_UP) == 0) {
        continue;
      }
      sa = (struct sockaddr *)(ifm + 1);
      get_rtaddrs(ifm->ifm_addrs, sa, rti_info);
      if ((sa = rti_info[RTAX_IFP]) != NULL) {
        ifi = Calloc(1, sizeof(struct ifi_info));
        *ifipnext = ifi;
        ifipnext = &ifi->ifi_next;

        ifi->ifi_flags = flags;
        if (sa->sa_family == AF_LINK) {
          sdl = (struct sockaddr_dl *)sa;
          ifi->ifi_index = sdl->sdl_index;
          if (sdl->sdl_nlen > 0) {
            snprintf(ifi->ifi_name, IFI_NAME, "%*s", sdl->sdl_nlen, &sdl->sdl_data[0]);
          }else {
            snprintf(ifi->ifi_name, IFI_NAME, "index %d", sdl->sdl_index);
          }
          if ((ifi->ifi_hlen = sdl->sdl_alen) > 0) {
            memcpy(ifi->ifi_haddr, LLADDR(sdl), min(IFI_HADDR, sdl->sdl_alen));
          }
        }
      }
    }else if (ifm->ifm_type == RTM_NEWADDR) {
      if (ifi->ifi_addr) {
        if (doaliases == 0) {
          continue;
        }
        ifisave = ifi;
        ifi = Calloc(1, sizeof(struct ifi_info));
        *ifipnext = ifi;
        ifipnext = &ifi->ifi_next;
        ifi->ifi_flags = ifisave->ifi_flags;
        ifi->ifi_index = ifisave->ifi_index;
        ifi->ifi_hlen = ifisave->ifi_hlen;
        memcpy(ifi->ifi_name, ifisave->ifi_name, IFI_NAME);
        memcpy(ifi->ifi_haddr, ifisave->ifi_haddr, IFI_HADDR);
      }
      ifam = (struct ifa_msghdr *)next;
      sa = (struct sockaddr *)(ifam + 1);
      get_rtaddrs(ifam->ifam_addrs, sa, rti_info);

      if ((sa = rti_info[RTAX_IFA]) != NULL) {
        ifi->ifi_addr = Calloc(1, sa->sa_len);
        memcpy(ifi->ifi_addr, sa, sa->sa_len);
      }
      if ((flags & IFF_BROADCAST) && (sa = rti_info[RTAX_BRD]) != NULL) {
        ifi->ifi_brdaddr = Calloc(1, sa->sa_len);
        memcpy(ifi->ifi_brdaddr, sa, sa->sa_len);
      }
      if ((flags & IFF_POINTOPOINT) && (sa = rti_info[RTAX_BRD]) != NULL) {
        ifi->ifi_dstaddr = Calloc(1, sa->sa_len);
        memcpy(ifi->ifi_dstaddr, sa, sa->sa_len);
      }
    }else {
      err_quit("unexpected message type %d", ifm->ifm_type);
    }
    return ifihead;
  }
}
