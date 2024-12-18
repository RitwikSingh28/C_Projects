#include <stdio.h>
 
void function(int a, int b, int c) {
  char buffer1[5];
  char buffer2[10];
  int *ret;

  ret = (int *)(buffer1 + 20);
  (*ret) += 8;
}

int main(void) {
  int x;

  x = 10;
  function(1, 2, 3);

  x = 1;
  printf("%d\n", x);

  return 0;
}
