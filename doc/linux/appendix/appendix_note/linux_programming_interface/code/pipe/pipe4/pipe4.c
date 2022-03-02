#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, const char* argv[]) {
  int fd[2];
  int ret = pipe(fd);

  if (ret == -1) {
    perror("pipe error");
    exit(1);
  }

  pid_t myid = getpid();
  printf("Parent ID is %ld\n", (long) myid);

  return 0;
}
