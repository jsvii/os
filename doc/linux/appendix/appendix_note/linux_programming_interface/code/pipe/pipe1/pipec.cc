#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#include <signal.h>		/* for SIG_ERR */

#define	MAXLINE	4096			/* max line length */

int
main(void)
{
  int		n;
  int		fd[2];
  pid_t	pid;
  char	line[MAXLINE];

  if (pipe(fd) < 0) {
    printf("pipe error");
  }

  if ((pid = fork()) < 0) {
    printf("fork error");
  } else if (pid == 0) {
    printf("child\n");               // 父进程写
    close(fd[0]);
    write(fd[1], "hello world\n", 12);  // 父进程的数据在fd[1]中写
  } else {
    printf("parent\n");           // 子进程读
    close(fd[1]);
    n = read(fd[0], line, MAXLINE);
    //    write(STDOUT_FILENO, line, n);
    printf("the n is: %d\n", n);
  }

  //sleep(20);
  exit(0);
}
