#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_file(const char *file_path, size_t *size) {
  FILE *fp = fopen(file_path, "rb");
  char *buf = NULL;

  if (!fp) {
    goto error;
  }

  if (fseek(fp, 0L, SEEK_END) < 0) {
    goto error;
  }

  long fsize = ftell(fp);
  if (fsize < 0) {
    goto error;
  }

  buf = (char *)malloc(sizeof(char) * fsize);

  if (!buf) {
    goto error;
  }

  if (fseek(fp, 0, SEEK_SET) < 0) {
    goto error;
  }

  size_t bytes_read = fread(buf, 1, fsize, fp);
  assert(bytes_read == (size_t)fsize);

  if (ferror(fp)) {
    goto error;
  }

  if (size) {
    *size = bytes_read;
  }

  fclose(fp);

  return buf;

error:
  if (fp) {
    fclose(fp);
  }

  if (buf) {
    free(buf);
    buf = NULL;
  }

  return NULL;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: ./excel_eng <input.csv>\n");
    fprintf(stderr, "ERROR: Input file not provided\n");
    exit(1);
  }

  const char *input_file_path = argv[1];

  size_t input_size = 0;
  char *data = read_file(input_file_path, &input_size);
  if (!data) {
    fprintf(stderr, "ERROR: Could not read file %s: %s\n", input_file_path,
            strerror(errno));
  }

  fwrite(data, 1, input_size, stdout);
  return 0;
}
