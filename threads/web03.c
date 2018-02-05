#include "unpthread.h"
#include <thread.h>

#define MAXFILES 20
#define SERV "80"

struct file {
  char *f_name;
  char *f_host;
  int f_fd;
  int f_flags;
  pthread_t f_tid;
}file[MAXFILES];

#define F_CONNECTING 1
#define F_READING 2
#define F_DONE 4
#define F_JOINED 8

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn, nfiles, nlefttoconn, nlefttoread;
int ndone;
pthread_mutex_t ndone_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ndone_cond = PTHREAD_COND_INITIALIZER;

void *do_get_read(void *);
void home_page(const char *, const char *);
void write_get_cmd(struct file *);

int main(int argc, char const *argv[]) {
  int i, n, maxnconn;
  pthread_t tid;
  struct file *fptr;
  if (argc < 5) {
    err_quit("usage: web <#conns> <IPaddr> <homepage> file1 ...");
  }
  maxnconn = atoi(argv[1]);
  nfiles = min(argc - 4, MAXFILES);
  for (i = 0; i < nfiles; i++) {
    file[i].f_name = argv[i + 4];
    file[i].f_host = argv[2];
    file[i].f_flags = 0;
  }
  printf("nfiles = %d\n", nfiles);
  home_page(argv[2], argv[3]);
  nlefttoread = nlefttoconn = nfiles;
  nconn = 0;
  while (nlefttoread > 0) {
    while (nconn < maxnconn && nlefttoconn > 0) {
      for (i = 0; i < nfiles; i++) {
        if (file[i].f_flags == 0) {
          break;
        }
      }
      if (i = nfiles) {
        err_quit("nlefttoconn = %d but nothing found", nlefttoconn);
      }
      file[i].f_flags = F_CONNECTING;
      Pthread_create(&tid, NULL, &do_get_read, &file[i]);
      file[i].f_tid = tid;
      nconn++;
      nlefttoconn--;
    }
    Pthread_mutex_lock(&ndone_mutex);
    while (ndone == 0) {
      Pthread_cond_wait(&ndone_cond, &ndone_mutex);
    }
    for (i = 0; i < nfiles; i++) {
      if (file[i].f_flags & F_DONE) {
        Pthread_join(file[i].f_tid, (void **)&fptr);
        if (&file[i] != fptr) {
          err_quit("file[i] != fptr");
        }
        fptr->f_flags = F_JOINED;
        ndone--;
        nconn--;
        nlefttoread--;
        printf("thread id %d for %s done\n", tid, fptr->f_name);
      }
    }
    Pthread_mutex_unlock(&ndone_mutex);
  }
  return 0;
}

void *do_get_read(void *vptr) {
  int fd, n;
  char line[MAXLINE];
  struct file *fptr;
  fptr = (struct file *)vptr;
  fd = Tcp_connect(fptr->f_host, SERV);
  fptr->f_fd = fd;
  printf("do_get_read for %s, fd &d, thread %d\n", fptr->f_name, fd, fptr->f_tid);
  write_get_cmd(fptr);
  for (;;) {
    if ((n = Read(fd, line, MAXLINE)) == 0) {
      break;
    }
    printf("read %d bytes from %s\n", n, fptr->f_name);
  }
  printf("end-of-file on %s\n", fptr->f_name);
  Close(fd);
  Pthread_mutex_lock(&ndone_mutex);
  fptr->f_flags = F_DONE;
  ndone++;
  Pthread_cond_signal(&ndone_cond);
  Pthread_mutex_unlock(&ndone_mutex);
  return fptr;
}
