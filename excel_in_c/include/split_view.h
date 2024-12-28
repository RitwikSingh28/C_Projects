#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stddef.h>

typedef struct {
  size_t count;
  const char *data;
}StringView;

StringView sv_gen(const char *data, size_t count);
StringView sv_split_by_delim(StringView *sv, char delim);
StringView sv_trim_left(StringView sv);
StringView sv_trim_right(StringView sv);
StringView sv_trim(StringView sv);

#endif
