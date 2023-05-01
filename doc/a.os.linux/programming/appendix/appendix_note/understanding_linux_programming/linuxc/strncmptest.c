#include <string.h>
#include <stdio.h>

int main(void)
{
  char *buf1 = "hello,leo";
  char *buf2 = "hello,World";
  int result1;
  int result2;

  result1 = strncmp(buf1, buf2, 5);

  printf("result1 is: %d.\n", result1);

  result2 = strcmp(buf1, buf2);

  printf("result2 is: %d.\n", result2);

  return 0;

}
