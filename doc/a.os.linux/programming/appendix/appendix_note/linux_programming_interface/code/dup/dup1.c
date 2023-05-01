#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, const char* args[]) {

  int newfd = dup(1);
  pid_t myid = getpid();

  while(1) {
    sleep(2);
    printf("newfd is %d\n", newfd);
    printf("pid is %ld\n", (long) myid);
  }

  return 0;
}
