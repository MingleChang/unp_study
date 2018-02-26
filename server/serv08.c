#include "unpthread.h"
#include "pthread08.h"

static int unpthreads;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[]) {
  int i, listenfd, connfd;
  void sig_int(int), thread_make(int);
  socklen_t addrlen, clilen;
  struct sockaddr *cliaddr;

  if (argc == 3) {
    listenfd = Tcp_listen(NULL, argv[1], &addrlen);
  }else if (argc == 4) {
    listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
  }else {
    err_quit("usage: serv08 [<host>] <port#> <#threads>");
  }
  cliaddr = Malloc(addrlen);

  nthreads = atoi(argv[argc - 1]);
  tptr = Calloc(nthreads, sizeof(Thread));
  iget = iput = 0;

  for (i = 0; i < nthreads; i++) {
    thread_make(i);
  }

  Signal(SIGINT, sig_int);

  for (;;) {
    clilen = addrlen;
    connfd = Accept(listenfd, cliaddr, &clilen);

    Pthread_mutex_lock(&clifd_mutex);
    clifd[iput] = connfd;
    if (++iput == MAXNCLI) {
      iput = 0;
    }
    if (iput == iget) {
      err_quit("iput = iget = %d", iput);
    }
    Pthread_cond_signal(&clifd_cond);
    Pthread_mutex_unlock(&clifd_mutex);
  }

  return 0;
}
