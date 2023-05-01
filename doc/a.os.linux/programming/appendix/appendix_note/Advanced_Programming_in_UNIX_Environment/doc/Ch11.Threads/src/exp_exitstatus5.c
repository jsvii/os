#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

void *
thr_fn1(void *arg)
{
  printf("thread 2 starting\n");
  sleep(5);
  printf("thread 2 exiting\n");
  pthread_exit((void *)2);
}

int
main(void)
{
  pthread_t tid1;
  int err;
  pid_t pid;

  pid = getpid();
  printf("pid is %u \n", (unsigned int) pid);

  err = pthread_create(&tid1, NULL, thr_fn1, NULL);
  if (err != 0)
    printf("can't create thread 1");


  if (err != 0) {
    printf("can't join with thread 1");
  }


  printf("----------------------\n");
  /* 通过 pthread_exit 退出主线路 */
  pthread_exit((void *)0);
}
