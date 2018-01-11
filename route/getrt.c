#include "unproute.h"

#define BUFLEN (sizeof(struct rt_msghdr) + 512)

#define SEQ 9999

int main(int argc, char const *argv[]) {
  int sockfd;
  char *buf;
  pid_t pid;
  ssize_t n;
  struct rt_msghdr *rtm;
  struct sockaddr *sa, *rti_info[RTAX_MAX];
  struct sockaddr_in *sin;

  return 0;
}
