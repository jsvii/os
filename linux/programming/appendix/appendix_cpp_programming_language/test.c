#include <stdio.h>

struct foo {
  int x;
  float y;
};


int main() {
    
    int a = 10;

    struct foo var1;
   // struct foo* pvar;
   //

    var1.x = 10;

    printf("var1.x is %d\n", var1.x);

    (&var1)->x = 20;


    printf("var1.x is %d\n", var1.x);

    printf("the number is %d\n", a);

    return 0;
}
