#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

void init_daemon(void)
{
  int pid;
  int i;

  if ( pid=fork() ) {
    //是父进程，结束父进程,摆脱会话组长身份。
    fprintf("parent fork id is %d", pid);
    exit(0);
  } else if(pid< 0) {
    //fork失败，退出
    exit(1);
  }

  //是第一子进程，后台继续执行
  //
  setsid();

  //第一子进程成为新的会话组长和进程组长
  //并与控制终 端分离
  if (pid=fork()) {
    fprintf("fork id is %d", pid);
    //是第一子进程，结束第一子进程
    exit(0);
  } else if(pid< 0) {
    exit(1);//fork失败，退出
  }

  //是第二子进程，继续
  //第二子进程不再是会话组长，也就不会请求打开控制终端。

  for(i=0;i< NOFILE;++i) {
    close(i);
  }

  //关闭打开的文件描述符

  chdir("/tmp"); //改变工作目录到/tmp

  umask(0);//重设 文件创建掩模

  return;

}
