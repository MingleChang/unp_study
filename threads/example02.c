#include "unp.h"

#define NLOOP 5000

int counter;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *doit(void *);

int main(int argc, char const *argv[]) {
  pthread_t tidA, tidB;
  Pthread_create(&tidA, NULL, &doit, NULL);
  Pthread_create(&tidB, NULL, &doit, NULL);
  Pthread_join(tidA, NULL);
  Pthread_join(tidB, NULL);
  return 0;
}

void *doit(void *vptr) {
  int i, val;
  for (i = 0; i < NLOOP; i++) {
    Pthread_mutex_lock(&counter_mutex);
    val = counter;
    printf("%d: %d\n", pthread_self(), val + 1);
    counter = val + 1;
    Pthread_mutex_unlock(&counter_mutex);
  }
  return NULL;
}
