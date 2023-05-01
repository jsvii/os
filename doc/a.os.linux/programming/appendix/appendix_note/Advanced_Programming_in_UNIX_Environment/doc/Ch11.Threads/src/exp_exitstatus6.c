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
  printf("thread 1 starting\n");

  printf("thread 1 exiting\n");
  printf("[LAST MESSAGE] this is the last message from  thread 1\n");
  exit(0);
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

  /* 本例尝试在线程中调用 exit，看是否退出进程 */
  sleep(10);
  printf("----------------------\n");

  pthread_exit((void *)0);
}
