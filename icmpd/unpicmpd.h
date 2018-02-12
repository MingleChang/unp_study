#ifndef __unpicmp_h
#define __unpicmp_h

#include "unp.h"

#define ICMPD_PATH "/tmp/icmpd"

struct icmpd_err {
  int icmpd_errno;
  char icmpd_type;
  char icmpd_code;
  socklen_t icmpd_len;
  struct sockaddr_storage icmpd_dest;
};

#endif
