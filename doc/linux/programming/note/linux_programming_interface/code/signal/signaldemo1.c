#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sigHandler(int signo) {
  printf("ouch!\n");
}

int main ( int argc, char *argv[] )
{
  int j;
  pid_t processId;
  processId = getpid();
  printf("process id is: %d\n", processId);


  if ( signal(SIGINT,sigHandler) == SIG_ERR ) {
    perror("signal\n");
  }

  for (j=0; ; j++) {
    printf("num is %d\n", j);
    sleep(3);
  }

  return 0;
}
