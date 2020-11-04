#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* args[])
{
  int count = 0;
  printf("the argument length is %d .\n", argc);

  for ( count = 0; count < argc; count++ ) {
    printf("The %d argument is %s.\n", count, args[count]);
  }

  return 0;
}
