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

  pid_t pid = fork();
  if (pid == -1) {
    perror("fork error");
    exit(1);
  }

  if (pid > 0) {
    pid_t myid = getpid();
    //    printf("Parent ID is %ld\n", (long) myid);
    //    printf("pid is %ld\n", (long) pid);

    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    execlp("ps", "ps", "aux", NULL);
    perror("execclp");
    exit(1);
  } else if (pid == 0) {
    pid_t myid2 = getpid();
    //    printf("child ID is %ld\n", (long) myid2);
    printf("pid is %ld\n", (long) myid2);

    close(fd[1]);
    dup2(fd[0], STDIN_FILENO);
    execlp("grep", "grep", "--color", "bash", NULL);
    printf("-----------------------\n");
    exit(1);
  }

  return 0;
}
