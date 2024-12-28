#ifndef DYN_ARRAY_H
#define DYN_ARRAY_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  size_t count;
  const char *data;
}StringView;

#define SV(str) sv_gen(str, sizeof(str) - 1)
#define SV_Fmt "%.*s"
#define SV_Arg(sv) (int) (sv).count, (sv).data

StringView sv_gen(const char *data, size_t count);
StringView sv_split_by_delim(StringView *sv, char delim);
StringView sv_trim_left(StringView sv);
StringView sv_trim_right(StringView sv);
StringView sv_trim(StringView sv);
bool sv_starts_with(StringView sv, StringView prefix);
bool sv_eq(StringView sv1, StringView sv2);

#endif
