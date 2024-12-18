#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void segfault_handler() {
  printf("Segfault captured!\n");
  exit(1);
}

int main() {
  void* initial_break = sbrk(0);
  if(initial_break == (void*) -1) {
    fprintf(stderr, "sbrk failed: %s\n", strerror(errno));
    exit(1);
  }
  printf("Initial Break Addr: %p\n", initial_break);

  void* new_break = sbrk(1024);
  if(new_break == (void*) -1) {
    fprintf(stderr, "sbrk failed: %s\n", strerror(errno));
    exit(1);
  }
  printf("New Break Addr: %p\n", new_break);

  int page_size = sysconf(_SC_PAGESIZE);
  if(page_size == -1) {
    fprintf(stderr, "sysconf failed: %s\n", strerror(errno));
    exit(1);
  }
  printf("Page Size: %d\n", page_size);

  // Calculating the address of the page boundary
  void* page_boundary = (void*)(((uintptr_t)new_break + page_size - 1) & ~(page_size - 1)) + page_size;
  printf("Page Boundary: %p\n", page_boundary);
  
  // Accessing the no-man's land
  char* no_mans_land = (char*)new_break + 1024; 
  *no_mans_land = 's';
  printf("Value written to no_mans_land: %c\n", *no_mans_land);

  if (signal(SIGSEGV, segfault_handler) == SIG_ERR){
    fprintf(stderr, "Error installing signal handler\n");
    exit(1);
  }

  // Access memory page above page boundary (should result in a SIGSEGV)
  char* invalid_addr = (char*) page_boundary + 1;
  printf("Attempting to access address above page boundary: %p\n", invalid_addr);
  
  *invalid_addr = 'y'; // should result in a SIGSEGV
  return 0;
}
