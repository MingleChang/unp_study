#include "unprtt.h"

int rtt_d_flag = 0;

#define RTT_RTOCALC(ptr) ((ptr)->rtt_srtt * (4.0 * (ptr)->rtt_rttvar))
static float rtt_minmax(float rto) {
  if (rto < RTT_RXTMIN) {
    rto = RTT_RXTMIN;
  }else if (rto > RTT_RXTMAX) {
    rto = RTT_RXTMAX;
  }
  return rto;
}

void rtt_init(struct rtt_info *ptr) {
  struct timeval tv;
  Gettimeofday(&tv, NULL);
  ptr->rtt_base = tv.tv_sec;
  ptr->rtt_rtt = 0;
  ptr->rtt_srtt = 0;
  ptr->rtt_rttvar = 0.75;
  ptr->rtt_rto = rtt_minmax(RTT_RTOCALC(ptr));
}
