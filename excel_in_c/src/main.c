#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "split_view.h"

#define INIT_CAP 5

typedef enum {
  EXPR_TYPE_NUMBER = 0,
  EXPR_TYPE_CELL,
  EXPR_TYPE_BINARY_OP,
} Expr_Type;

typedef struct {
  Expr_Type type;
} Expr;

typedef enum {
  CELL_TYPE_TEXT = 0,
  CELL_TYPE_NUMBER,
  CELL_TYPE_EXPR,
} Cell_Type;

typedef union {
  StringView text;
  double number;
  Expr expr;
} Cell_Value;

typedef struct {
  Cell_Type type;
  Cell_Value value;
} Cell;

typedef struct {
  Cell *cells;
  size_t rows;
  size_t cols;
} Table;

Cell *table_cell_at(Table *table, size_t row, size_t col) {
  assert(row < table->rows );
  assert(col < table->cols);
  return &table->cells[row * table->cols + col];
}

Table table_alloc(size_t rows, size_t cols) {
  Table table = {0};
  table.rows = rows;
  table.cols = cols;

  table.cells = malloc(sizeof(Cell) * rows * cols);
  if (!table.cells) {
    fprintf(stderr, "ERROR: Failed to allocate Table struct.\n");
    exit(EXIT_FAILURE);
  }

  memset(table.cells, 0, sizeof(Cell) * rows * cols);
  return table;
}

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

void find_table_size(StringView content, size_t *max_rows, size_t *max_cols) {
  size_t rows = 0;
  size_t cols = 0;
  for (; content.count > 0; ++rows) {
    StringView line = sv_split_by_delim(&content, '\n');
    size_t col = 0;
    for (; line.count > 0; ++col) {
      sv_trim(sv_split_by_delim(&line, '|'));
    }

    cols = col > cols ? col : cols;
  }

  if (max_rows) {
    *max_rows = rows;
  }
  if (max_cols) {
    *max_cols = cols;
  }
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

  StringView input_view = {
    .data = data,
    .count = input_size,
  };

  size_t rows = 0;
  size_t cols = 0;
  find_table_size(input_view, &rows, &cols);
  printf("Input Dimensions: %zu x %zu\n", rows, cols);

  Table table = table_alloc(rows, cols);

  return 0;
}
