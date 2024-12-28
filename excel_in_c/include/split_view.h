#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stddef.h>

typedef struct {
  size_t count;
  const char* data;
}StringView;

StringView gen_sv(const char* data, size_t count);
StringView split_by_delim(StringView* sv, char delim);

#endif
