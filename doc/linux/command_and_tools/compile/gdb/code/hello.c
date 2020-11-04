#include <stdio.h>

int main(int argc, const char *argv[]) {
  int total = 0;
  int b = 0;

  for (b = 1; b < 100; b++ ) {
    total = total + b;
  }

  printf("Result is %d.\n", total);

  return 0;
}
