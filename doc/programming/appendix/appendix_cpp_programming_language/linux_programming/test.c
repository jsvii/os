#include <stdio.h>
#include <time.h>

void init_daemon(void);//守护进程初始化函数

int main()
{

  printf("this is a test");

  FILE *fp;
  time_t t;

  //初始化为Daemon
  printf("init daemon");
  init_daemon();
  printf("init daemon 2");
  //每隔一分钟向test.log报告运行状态

  while(1) {
    //睡眠一分钟
    sleep(10);

    printf("this is step A");

    if((fp=fopen("test.log","a")) >=0) {
      t=time(0);

      fprintf(fp,"Im here at %sn",asctime(localtime(&t)) );

      fclose(fp);
    }
    printf("this is step B");
  }

  printf("this is a test Z");

}
