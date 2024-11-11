/*
 * We are going to use sbrk for malloc implementation
 * The OS reserves stack and heap space for processes and sbrk lets us manipulate the heap.
 * sbrk(0) returns the current top of the heap.
 * sbrk(foo) increments the heap size by foo and returns a pointer to the previous top 
 */

#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>

struct BlockMeta{
  size_t size;
  struct BlockMeta *next;
  int free;
  int magic; // For debugging purposes; TODO: Remove in non-debug mode
};

#define META_SIZE sizeof(struct BlockMeta)

void *global_base = NULL; // The head of our linkedlist of memory blocks
/*
 * `**last` to keep track of the previous block while we're traversing the list of blocks 
 * Leads to an efficient traversal and insertion of the new blocks in the ll
 */
struct BlockMeta* find_free_block(struct BlockMeta **last, size_t size) {
  struct BlockMeta *current = global_base;
  while(current && !(current->free && current->size >= size)) {
    *last = current;
    current = current->next;
  }
  return current;
}

/*
 * Just a Helper Function, does not return the pointer to the allocated memory
 * to the client directly. Need to modify the pointer to only point to the
 * requested size, and not the metadata
 */
struct BlockMeta* request_space(struct BlockMeta *last, size_t size) {
  struct BlockMeta* block = sbrk(0);
  void* request = sbrk(size + META_SIZE);

  assert((void*)block == request); // not thread-safe
  if(request == (void*) -1) {
    return NULL;
  } 

  if(last) {
    last->next = block;
  }

  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}

/*
 * Given a pointer to a memory block, return a pointer that points from the 
 * starting of the associated metadata for that particular block
 */
struct BlockMeta *get_block_ptr(void *ptr) {
  return (struct BlockMeta*)ptr - 1;
}

void* malloc(size_t size) {
  struct BlockMeta *block;

  if(size <= 0) {
    return NULL;
  }

  if(!global_base) { // First call
    block = request_space(NULL, size);
    if(!block) {
      return NULL;
    }
    global_base = block;
  } else {
    struct BlockMeta *last = global_base;
    block = find_free_block(&last, size);
    if(!block) { // Couldn't find a suitable free block
      block = request_space(last, size);
      if(!block) {
        return NULL;
      }
    } else { // found a free block
      block->free = 0;
      block->magic = 0x77777777;
    }
  }

  // return a pointer to the region after block_meta
  // +1 increments the address by one sizeof(struct BlockMeta)
  return (block + 1); 
}

void free(void* ptr) {
  if(!ptr) return;

  struct BlockMeta *block_ptr = get_block_ptr(ptr);
  assert(block_ptr->free == 0);
  assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678); // remove for non-debug environment
  block_ptr->free = 1;
  block_ptr->magic = 0x55555555;
}

void* realloc(void* ptr, size_t size) {
  if(!ptr) {
    // NULL pointer, realloc should act like malloc
    return malloc(size);
  }

  struct BlockMeta *block_ptr = get_block_ptr(ptr);
  if(block_ptr->size >= size) {
    // Enough space available already
    return ptr;
  }

  // Need to realloc. Malloc new space and free old space.
  // Then copy the data to the new space
  void *new_ptr;
  new_ptr = malloc(size);
  if(!new_ptr) {
    return NULL;
  }

  memcpy(new_ptr, ptr, block_ptr->size);
  free(ptr);
  return new_ptr;
}

void *calloc(size_t nelem, size_t elsize) {
  size_t size = nelem * elsize;
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}
