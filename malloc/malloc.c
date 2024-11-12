/*
 * We are going to use sbrk for malloc implementation
 * The OS reserves stack and heap space for processes and sbrk lets us manipulate the heap.
 * sbrk(0) returns the current top of the heap.
 * sbrk(foo) increments the heap size by foo and returns a pointer to the previous top 
 */

#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>

struct BlockMeta{
  size_t size;
  struct BlockMeta *next;
  struct BlockMeta *prev; // This would be useful for merging together adjacent free blocks for both ahead and behind
  int free;
  int magic; // For debugging purposes; TODO: Remove in non-debug mode
};

#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1)) // Rounding up to the nearest multiple of ALIGNMENT
#define META_SIZE ALIGN(sizeof(struct BlockMeta))

void *global_base = NULL; // The head of our linkedlist of memory blocks

/*
 * `**last` to keep track of the previous block while we're traversing the list of blocks 
 * Leads to an efficient traversal and insertion of the new blocks in the linkedlist
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
 * eequested size, and not the metadata
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
    block->prev = last;
  } else {
    block->prev = NULL;
  }

  block->size = size;
  block->next = NULL;
  block->free = 0;
  block->magic = 0x12345678;
  return block;
}

/*
 * Given a pointer to a memory block, return a pointer that points to the 
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

  size = ALIGN(size);
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
      if ((block->size - size) >= (META_SIZE + ALIGNMENT)) {
        // split the block 
        struct BlockMeta *split = (struct BlockMeta *)((char*)block + META_SIZE + size);

        // Initialize the fields of the `split` block
        split->size = block->size - size - META_SIZE;
        split->next = block->next;
        if (split->next) {
          split->next->prev = split;
        }
        split->prev = block;
        split->free = 1;
        split->magic = 0x55555555;

        // Updating the original block
        block->next = split;
        block->size = size;
      }

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

  // -------------------------------------------------------------------------
  // coalescing adjacent free blocks together into one to avoid fragmentation
  // -------------------------------------------------------------------------
  
  // Merge with the previous block if free
  if (block_ptr->prev && block_ptr->prev->free == 1) {
    block_ptr->prev->size += (META_SIZE + block_ptr->size);
    block_ptr->prev->next = block_ptr->next;
    if (block_ptr->next) {
      block_ptr->next->prev = block_ptr->prev;
    }
    block_ptr = block_ptr->prev; // Move back to the previous block after merging
  } 
  // Merge with the next block if free
  if (block_ptr->next && block_ptr->next->free == 1) {
    block_ptr->size += (META_SIZE + block_ptr->next->size); 
    block_ptr->next = block_ptr->next->next;
    if (block_ptr->next) {
      block_ptr->next->prev = block_ptr;
    }
  }
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

  memcpy(new_ptr, ptr, size < block_ptr->size ? size : block_ptr->size); // A check to avoid memory corruption if the new size is smaller than the original one
                                                                         // Only need to write the minimum of the old and the new sizes
  free(ptr);
  return new_ptr;
}

void *calloc(size_t nelem, size_t elsize) {
  if (nelem && (elsize > (SIZE_MAX / nelem))) {
    return NULL; // Overflow
  }
  size_t size = nelem * elsize;
  void *ptr = malloc(size);
  memset(ptr, 0, size);
  return ptr;
}
