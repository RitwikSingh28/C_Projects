#include "dyn_array.h"
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

int split_lines(StringArray *dest, const char* buffer) {
  // Converting the data into a [*char] by splitting off on `\n`
  char line[1024];
  size_t index = 0;

  for (const char* start = buffer; *start != '\0'; start++) {
    if (*start == '\n') {
      line[index] = '\0';
      if (add_line(dest, line) < 0) {
        fprintf(stderr, "ERROR: Failure in adding line to StringArray.\n");
        return -1;
      }
      index = 0;  // Reset index for the next line
    } else {
      if (index < sizeof(line) - 1) {
        line[index++] = *start;
      } else {
        fprintf(stderr, "ERROR: Line exceeds buffer size\n");
        return -1;
      }
    }
  }

  if (index > 0) {
    line[index] = '\0';
    if (add_line(dest, line) < 0) {
      fprintf(stderr, "ERROR: Failure in adding line to StringArray.\n");
      return -1;
    }
  }

  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: ./excel_eng <input.csv>\n");
    fprintf(stderr, "ERROR: Input file not provided\n");
    exit(EXIT_FAILURE);
  }

  const char *input_file_path = argv[1];

  size_t input_size = 0;
  char *data = read_file(input_file_path, &input_size);
  if (!data) {
    fprintf(stderr, "ERROR: Could not read file %s: %s\n", input_file_path,
            strerror(errno));
  }

  StringArray *lines = create_array(5);
  if (!lines) {
    fprintf(stderr, "ERROR: Could not get StringArray Struct\n");
    exit(EXIT_FAILURE);
  }

  if (split_lines(lines, data) != 0) {
    fprintf(stderr, "ERROR: Failed to split lines\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < lines->size; i++) {
    fprintf(stdout, "Line %zu: %s\n", i, lines->line[i]);
  }

  return 0;
}
