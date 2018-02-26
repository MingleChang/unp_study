#include "unpthread.h"
#include "pthread07.h"

pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char *argv[]) {
  int i;
  void sig_int(int), thread_make(int);

  if (argc == 3) {
    listenfd = Tcp_listen(NULL, argv[1], &addrlen);
  }else if (argc == 4) {
    listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  }else {
    err_quit("usage: serv07 [<host>] <port#> <#threads>");
  }
  nthreads = atoi(argv[argc - 1]);
  tptr = Calloc(nthreads, sizeof(Thread));
  for (i = 0; i < nthreads; i++) {
    thread_make(i);
  }
  Signal(SIGINT, sig_int);

  for (;;) {
    pause();
  }

  return 0;
}
