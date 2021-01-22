#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

static void sig_int(int signo) {
  printf("catch SIGINT\n");
  if (signal(SIGINT, SIG_DFL) == SIG_ERR ) {
    perror("signal\n");
  }
}

int main ( int argc, char *argv[] )
{
  sigset_t newset,oldset,pendmask;

  if ( signal(SIGINT,sig_int) == SIG_ERR ) {
    perror("signal\n");
  }

  if ( sigemptyset(&newset) < 0 ) {
    perror("sigempty\n");
  }

  if ( sigaddset(&newset, SIGINT) < 0 ) {
    perror("sigaddset\n");
  }

  if ( sigprocmask(SIG_BLOCK, &newset, &oldset) < 0 ) {
    perror("sigprocmask\n");
  }

  printf("\nSIGINT block\n");

  sleep(5);

  if ( sigpending(&pendmask) < 0) {
    perror("sigpending\n");
  }

  if ( sigismember(&pendmask, SIGINT) ) {
    printf("SIGINT is pendding\n");
  }

  if ( sigprocmask(SIG_SETMASK, &oldset, NULL) < 0 ) {
    perror("sigprocmask\n");
  }
  printf("\nSIGINT unblock\n");

  sleep(5);
  return 0;
}
