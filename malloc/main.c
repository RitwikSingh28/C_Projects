#include <stdio.h>
#include <stdlib.h>

int main() {
  
  typedef struct {
    char x;
    int y;
  }Sample;

  Sample* x = (Sample*)malloc(sizeof(Sample));
  x->x = 21;
  x->y = 123;
  
  printf("Address of struct: %p\n", x);
  printf("Address of x: %p\n", &(x->x));
  printf("Address of y: %p\n", &(x->y));
  free(x);
  return 0;
}
