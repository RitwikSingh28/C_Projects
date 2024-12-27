#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "USAGE: ./excel_eng <input.csv>\n");
    fprintf(stderr, "ERROR: Input file not provided\n");
    exit(1);
  }

  const char* input_file_path = argv[1];
  return 0;
}
