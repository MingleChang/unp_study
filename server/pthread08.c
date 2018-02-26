#include "unpthread.h"
#include "pthread08.h"

void thread_make(int i) {
  void *thread_main(void *);
  Pthread_create(&tptr[i].thread_tid, NULL, &thread_main, (void *)i);
  return;
}
