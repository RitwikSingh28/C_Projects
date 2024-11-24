#include <stdio.h>
#include <unistd.h>

typedef struct {
  
}Scheduler;

void countdown(int n) {
  while (n > 0) {
    printf("Count Down: %d\n", n);
    n--;
    sleep(1); 
  }
}

void countup(int n) {
  int x = 0;
  while (x < n) {
    printf("Count Up: %d\n", x);
    x++;
    sleep(1); 
  }
}

int main(void) {
  countdown(5);
  countup(5);
  return 0;
}
