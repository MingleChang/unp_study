#include "udpcksum.h"
#include <setjmp.h>

static sigjmp_buf jmpbuf;
static int canjump;

void sig_alrm(int signo) {
  if (canjump == 0) {
    return;
  }
  siglongjmp(jmpbuf, 1);
}

void test_udp(void) {
  volatile int nsent = 0, timeout = 3;
  struct udpiphdr *ui;
  Signal(SIGALRM, sig_alrm);
  if (sigsetjmp(jmpbuf, 1)) {
    if (nsent >= 3) {
      err_quit("no response");
    }
    printf("timeout\n");
    timeout *= 2;
  }
  canjump = 1;
  send_dns_query();
  nsent++;

  alarm(timeout);
  ui = udp_read();
  canjump = 0;
  alarm(0);

  if (ui->ui_sum == 0) {
    printf("UDP checksums off\n");
  }else {
    printf("UDP checksums on\n");
  }
  if (verbose) {
    printf("received UDP checksum = %x\n", ntohs(ui->ui_sum));
  }
}
