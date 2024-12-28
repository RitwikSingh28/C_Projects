#include "split_view.h"
#include <ctype.h>

StringView sv_gen(const char* data, size_t count) {
  StringView view;
  view.data = data;
  view.count = count;

  return view;
}

StringView sv_split_by_delim(StringView* sv, char delim) {
  size_t delim_pos = 0;
  
  while (delim_pos < sv->count && sv->data[delim_pos] != delim) {
    delim_pos += 1;
  }

  StringView result = sv_gen(sv->data, delim_pos);

  if (delim_pos < sv->count) {
    sv->data += delim_pos + 1;
    sv->count -= delim_pos + 1;
  } else {
    sv->count -= delim_pos;
    sv->data += delim_pos;
  }

  return result;
}

StringView sv_trim_left(StringView sv) {
  size_t i = 0;
  while (i < sv.count && isspace(sv.data[i])) {
    i += 1;
  }

  return sv_gen(sv.data + i, sv.count - i);
}

StringView sv_trim_right(StringView sv) {
  size_t i = 0;
  while (i < sv.count && isspace(sv.data[sv.count - i - 1])) {
    i++;
  }

  return sv_gen(sv.data, sv.count - i);
}

StringView sv_trim(StringView sv) {
  return sv_trim_right(sv_trim_left(sv));
}

