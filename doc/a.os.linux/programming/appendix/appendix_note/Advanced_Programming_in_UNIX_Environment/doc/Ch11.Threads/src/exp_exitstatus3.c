#include "apue.h"
#include <pthread.h>

pthread_t   tid1, tid2;

void *
thr_fn1(void *arg)
{
  void *tret;
  int err;

  printf("thread 1 starting\n");

  err = pthread_join(tid2, &tret);
  if (err != 0)
    err_quit("can't join with thread 2: %s\n", strerror(err));

  printf("thread 2 exit code %d\n", (int)tret);

  printf("thread 1 returning\n");
  return((void *)1);
}

void *
thr_fn2(void *arg)
{
  printf("thread 2 starting\n");
  sleep(1);
  printf("thread 2 exiting\n");
  pthread_exit((void *)2);
}

int
main(void)
{
  int err;
  void *tret;
  pid_t pid;

  pid = getpid();
  printf("pid is %u \n", (unsigned int) pid);

  err = pthread_create(&tid2, NULL, thr_fn2, NULL);
  if (err != 0)
    err_quit("can't create thread 2: %s\n", strerror(err));

  err = pthread_create(&tid1, NULL, thr_fn1, NULL);
  if (err != 0)
    err_quit("can't create thread 1: %s\n", strerror(err));

  sleep(2);

  printf("----------------------\n");

  return 0;
}
