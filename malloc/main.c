#include <stdlib.h>
#include <stdio.h>

int main(void) {
  int* ptr = (int*)malloc(sizeof(int));
  *ptr = 10;
  printf("Value: %d\n", *ptr);
  free(ptr);
  free(ptr);
  return 0;
}
