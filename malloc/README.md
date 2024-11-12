### Description

A self-implementation of `malloc` in C, stored in the file `malloc.c`. Also included is a sample `main.c` file that is to be used for testing purposes.

This implementation uses [`sbrk`](https://man7.org/linux/man-pages/man2/sbrk.2.html) to allocate memory internally. This is not at all optimal as of now, though work will continue on this.

Next Steps:
- Make our `malloc` return a pointer "which is suitably aligned for any built-in type"
- Split up the blocks to use the minimum amount of space
- Merge adjacent free blocks together into one
- Use the best-fit approach to re-allocate free blocks 

--- Running Tests

1. Compile `malloc.c` into `malloc`, with the following commands:
```
clang -O3 -g malloc.c -o malloc
```
or with `gcc`, if you prefer:
```
gcc -O3 -g malloc.c -o malloc
```
2. Run `malloc` to run the tests.

---
#### Steps For Using Custom Malloc

1. Compile `main.c` into  `main` using any C compiler of your choice.
2. Compile `malloc.c` into `malloc.so`. A sample command would be using `clang`:
```
clang -O0 -g -W -Wall -Wextra -shared -fPIC malloc.c -o malloc.so
```
3. Set the environment variable `LD_PRELOAD` to point to the full location of the `malloc.c` file in your local machine, i.e.
```
export LD_PRELOAD=<path_to_malloc.so>
```
4. Run the `main` binary, and see it work. You may modify the source code to include more advanced usage of `malloc` and `free` defined in custom `malloc.c` 

---
### Note

- This will most definitely contain bugs, so obviously prone to crash.

---
### References

- [Article](https://danluu.com/malloc-tutorial/)
- [LD_PRELOAD](https://jvns.ca/blog/2014/11/27/ld-preload-is-super-fun-and-easy/)
- [A more detailed implementation - dlmalloc](https://github.com/ennorehling/dlmalloc/blob/master/malloc.c)
- [Data Structure Alignment](https://danluu.com/malloc-tutorial/)
