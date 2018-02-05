#include "unp.h"

#define NLOOP 5000

int counter;

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
    val = counter;
    printf("%d: %d\n", pthread_self(), val + 1);
    counter = val + 1;
  }
  return NULL;
}
