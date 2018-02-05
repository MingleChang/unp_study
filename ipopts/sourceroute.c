#include "unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>

static u_char *optr;
static u_char *lenptr;
static int ocnt;

u_char *inet_strcrt_init(int type) {
  optr = Malloc(44);
  bzero(optr, 44);
  ocnt = 0;
  *optr++ = IPOPT_NOP;
  *optr++ = type ? IPOPT_SSRR : IPOPT_LSRR;
  lenptr = optr++;
  *optr++ = 4;
  return optr - 4;
}

int inet_srcrt_add(char *hostptr) {
  int len;
  struct addrinfo *ai;
  struct sockaddr_in *sin;
  if (ocnt > 9) {
    err_quit("too many source routes with: %s", hostptr);
  }
  ai = Host_serv(hostptr, NULL, AF_INET, 0);
  sin = (struct sockaddr_in *)ai->ai_addr;
  memcpy(optr, &sin->sin_addr, sizeof(struct in_addr));
  freeaddrinfo(ai);
  optr += sizeof(struct in_addr);
  ocnt++;
  len = 3 + (ocnt * sizeof(struct in_addr));
  *lenptr = len;
  return len + 1;
}

void inet_srcrt_print(u_char *ptr, int len) {
  u_char c;
  char str[INET_ADDRSTRLEN];
  struct in_addr hopl;
  memcpy(&hopl, ptr, sizeof(struct in_addr));
  while ((c = *ptr++) == IPOPT_NOP) {

  }
  if (c == IPOPT_LSRR) {
    printf("received LSRR: ");
  }else if (c == IPOPT_SSRR) {
    printf("received SSRR: ");
  }else {
    printf("received option type %d\n", c);
    return;
  }
  printf("%s\n", Inet_ntop(AF_INET, &hopl, str, sizeof(str)));
  len = *ptr++ - sizeof(struct in_addr);
  ptr++;
  while (len > 0) {
    printf("%s\n", Inet_ntop(AF_INET, ptr, str, sizeof(str)));
    ptr += sizeof(struct in_addr);
    len -= sizeof(struct in_addr);
  }
  printf("\n");
}
