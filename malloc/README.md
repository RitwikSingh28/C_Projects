### Description

A self-implementation of `malloc` in C, stored in the file `malloc.c`. Also included is a sample `main.c` file that is to be used for testing purposes.

#### Steps For Using

1. Compile `main.c` into  `main` using any C compiler of your choice.
2. Compile `malloc.c` into `malloc.so`. A sample command would be using `clang`:
```
clang -O0 -g -W -Wall -Wextra -shared -fPIC malloc.c -o malloc.so
```
2. Set the environment variable `LD_PRELOAD` to point to the full location of the `malloc.c` file in your local machine, i.e.
```
export LD_PRELOAD=<path_to_malloc.so>
```
3. Run the `main` binary, and see it work. You may modify the source code to include more advanced usage of `malloc` and `free` defined in custom `malloc.c` 


### Note

- This will most definitely contain bugs, so obviously prone to crash.

### References

- [Article](https://danluu.com/malloc-tutorial/)
- [LD_PRELOAD](https://jvns.ca/blog/2014/11/27/ld-preload-is-super-fun-and-easy/)
- [A more detailed implementation - dlmalloc](https://github.com/ennorehling/dlmalloc/blob/master/malloc.c)
