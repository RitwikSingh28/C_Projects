#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *file_path, size_t *size) {
  FILE *fp = fopen(file_path, "rb");
  char *buf = NULL;

  if (!fp) {
    return NULL;
  }

  if (fseek(fp, 0L, SEEK_END) < 0) {
    if (fp) {
      fclose(fp);
    }
    return NULL;
  }

  long fsize = ftell(fp);
  if (fsize < 0) {
    if(fp) {
      fclose(fp);
    }
    return NULL;
  }

  buf = (char *)malloc(sizeof(char) * fsize);

  if (!buf) {
    if (fp) {
      fclose(fp);
    }
  }

  if (fseek(fp, 0, SEEK_SET) < 0) {
    if (fp) {
      fclose(fp);
    }
    if (buf) {
      free(buf);
      buf = NULL;
    }
  }

  (void) file_path;
  (void) size;
  assert(0 && "Not Implemented!");
  return 0;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: ./excel_eng <input.csv>\n");
    fprintf(stderr, "ERROR: Input file not provided\n");
    exit(1);
  }

  const char* input_file_path = argv[1];

  size_t input_size = 0;
  char* data = read_file(input_file_path, &input_size);
  if (!data) {
    fprintf(stderr, "ERROR: Could not read file %s: %s\n", input_file_path, strerror(errno));
  }
  return 0;
}
