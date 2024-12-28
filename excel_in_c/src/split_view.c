#include "split_view.h"

StringView gen_sv(const char* data, size_t count) {
  StringView view;
  view.data = data;
  view.count = count;

  return view;
}

StringView split_by_delim(StringView* sv, char delim) {
  size_t delim_pos = 0;
  
  while (delim_pos < sv->count && sv->data[delim_pos] != delim) {
    delim_pos += 1;
  }

  StringView result = gen_sv(sv->data, delim_pos);

  if (delim_pos < sv->count) {
    sv->data += delim_pos + 1;
    sv->count -= delim_pos + 1;
  } else {
    sv->count -= delim_pos;
    sv->data += delim_pos;
  }

  return result;
}

