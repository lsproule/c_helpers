#ifndef helpers_h
#define helpers_h
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef HELPERS_FREE
#include <stdlib.h>
#define HELPERS_FREE free
#endif

#ifndef HELPER_ASSERT
#include <assert.h>
#define HELPER_ASSERT assert
#endif /* NOB_ASSERT */

#ifdef __cplusplus
#define HELPERS_DECLTYPE_CAST(T) (decltype(T))
#else
#define HELPERS_DECLTYPE_CAST(T)
#endif

#ifndef HELPER_FN
#define HELPER_FN // static inline
#endif

#ifndef HELPERS_REALLOC
#include <stdlib.h>
#define HELPERS_REALLOC realloc
#endif
#ifndef HELPERS_ARR_CAP
#define HELPERS_ARR_CAP 16
#endif

#define HELPERS_TEMP_CAPACITY 2048
static char helpers_temp[HELPERS_TEMP_CAPACITY];
static size_t helpers_temp_size = 0;

// Dynamic array macros

#define helpers_arr_reserve(arr, expected_capacity)                            \
  do {                                                                         \
    if ((expected_capacity) > (arr)->capacity) {                               \
      if ((arr)->capacity == 0) {                                              \
        (arr)->capacity = HELPERS_ARR_CAP;                                     \
      }                                                                        \
      while ((expected_capacity) > (arr)->capacity) {                          \
        (arr)->capacity *= 2;                                                  \
      }                                                                        \
      (arr)->items = HELPERS_DECLTYPE_CAST((arr)->items) HELPERS_REALLOC(      \
          (arr)->items, (arr)->capacity * sizeof(*(arr)->items));              \
      HELPER_ASSERT((arr)->items != NULL && "404 ram not found");              \
    }                                                                          \
  } while (0)

#define helpers_arr_append(arr, item)                                          \
  do {                                                                         \
    helpers_arr_reserve((arr), (arr)->count + 1);                              \
    (arr)->items[(arr)->count++] = (item);                                     \
  } while (0)

#define helpers_arr_multiappend(arr, new_items, len_new_items)                 \
  do {                                                                         \
    helpers_arr_reserve((arr), (arr)->count + (len_new_items));                \
    memcpy((arr)->items + (arr)->count, (new_items),                           \
           (len_new_items) * sizeof(*(arr)->items));                           \
    (arr)->count += (len_new_items);                                           \
  } while (0)

#define helpers_arr_free(arr) HELPERS_FREE((arr).items)

#define helpers_arr_size(arr) ((arr).count)
#define helpers_arr_capacity(arr) ((arr).capacity)

// String types
typedef struct {
  char *items;
  size_t count;
  size_t capacity;
} HelperString;

typedef struct {
  const char *items;
  size_t count;
} HelperStringView;

// Function declarations
HELPER_FN HelperString helpers_str_from_cstr(const char *);
HELPER_FN bool helpers_read_file(const char *path, HelperString *out);
HELPER_FN HelperStringView helpers_str_trim_left(HelperStringView str);
HELPER_FN HelperStringView helpers_str_trim_right(HelperStringView str);
HELPER_FN HelperStringView helpers_str_trim(HelperStringView str);
HELPER_FN bool helpers_str_view_eq(HelperStringView a, HelperStringView b);
HELPER_FN bool helpers_str_view_eq_cstr(HelperStringView a, const char *b);
HELPER_FN bool helpers_str_view_starts_with(HelperStringView str,
                                            const char *prefix);
HELPER_FN bool helpers_str_view_ends_with(HelperStringView str,
                                          const char *suffix);
HELPER_FN bool helpers_file_exists(const char *path);
HELPER_FN bool helpers_dir_exists(const char *path);
HELPER_FN bool helpers_create_dir(const char *path);
HELPER_FN bool helpers_write_file(const char *path, HelperStringView data);
HELPER_FN bool helpers_copy_file(const char *src_path, const char *dst_path);
HELPER_FN HelperStringView nob_temp_file_ext(const char *path);

#define HELPERS_IMPLEMENTATION
#ifdef HELPERS_IMPLEMENTATION
// HELPERS IMPLEMENTATION

// String helpers

HELPER_FN void *helper_temp_alloc(size_t requested_size) {
  size_t word_size = sizeof(uintptr_t);
  size_t size = (requested_size + word_size - 1) / word_size * word_size;
  if (helpers_temp_size + size > HELPERS_TEMP_CAPACITY)
    return NULL;
  void *result = &helpers_temp[helpers_temp_size];
  helpers_temp_size += size;
  return result;
}

HELPER_FN HelperString helpers_str_from_cstr(const char *cstr) {
  HelperString str = {0};
  size_t len = strlen(cstr);
  helpers_arr_reserve(&str, len);
  memcpy(str.items, cstr, len);
  str.count = len;
  return str;
}

HELPER_FN HelperStringView helpers_str_view_from_parts(const char *data,
                                                       size_t len) {
  HelperStringView view = {0};
  view.items = data;
  view.count = len;
  return view;
}

HELPER_FN bool helpers_read_file(const char *path, HelperString *out) {
  FILE *file = fopen(path, "rb");
  if (!file) {
    return false;
  }
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  helpers_arr_reserve(out, size);
  fread(out->items, sizeof(char), size, file);
  out->count = size;
  fclose(file);
  return true;
}

bool is_space(char c) { return c == ' ' || c == '\t'; }

HELPER_FN HelperStringView helpers_str_trim_left(HelperStringView str) {
  size_t i = 0;
  while (i < str.count && is_space(str.items[i])) {
    i++;
  }
  return helpers_str_view_from_parts(str.items + i, str.count - i);
}

HELPER_FN HelperStringView helpers_str_trim_right(HelperStringView str) {
  size_t i = str.count;
  while (i > 0 && is_space(str.items[i - 1])) {
    i--;
  }
  return helpers_str_view_from_parts(str.items, i);
}

HELPER_FN HelperStringView helpers_str_trim(HelperStringView str) {
  return helpers_str_trim_right(helpers_str_trim_left(str));
}

HELPER_FN bool helpers_str_view_eq(HelperStringView a, HelperStringView b) {
  if (a.count != b.count) {
    return false;
  }
  return memcmp(a.items, b.items, a.count) == 0;
}

HELPER_FN bool helpers_str_view_eq_cstr(HelperStringView a, const char *b) {
  size_t len_b = strlen(b);
  if (a.count != len_b) {
    return false;
  }
  return memcmp(a.items, b, a.count) == 0;
}
HELPER_FN bool helpers_str_view_starts_with(HelperStringView str,
                                            const char *prefix) {
  size_t len_prefix = strlen(prefix);
  if (str.count < len_prefix) {
    return false;
  }
  return memcmp(str.items, prefix, len_prefix) == 0;
}

HELPER_FN bool helpers_str_view_ends_with(HelperStringView str,
                                          const char *suffix) {
  size_t len_suffix = strlen(suffix);
  if (str.count < len_suffix) {
    return false;
  }
  return memcmp(str.items + str.count - len_suffix, suffix, len_suffix) == 0;
}

// Filesystem helpers

HELPER_FN bool helpers_file_exists(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file) {
    fclose(file);
    return true;
  }
  return false;
}
#ifndef _WIN32
#include <errno.h>
#include <sys/stat.h>
#endif

HELPER_FN bool helpers_dir_exists(const char *path) {
#ifdef _WIN32
  DWORD attr = GetFileAttributesA(path);
  return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
#else
  struct stat st;
  if (stat(path, &st) == 0) {
    return S_ISDIR(st.st_mode);
  }
  return false;
#endif
}

HELPER_FN bool helpers_create_dir(const char *path) {
#ifdef _WIN32
  return CreateDirectoryA(path, NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
#else
  struct stat st;
  if (stat(path, &st) == 0) {
    return S_ISDIR(st.st_mode);
  }
  return mkdir(path, 0755) == 0 || errno == EEXIST;
#endif
}

HELPER_FN bool helpers_write_file(const char *path, HelperStringView data) {
  FILE *file = fopen(path, "wb");
  if (!file) {
    return false;
  }
  size_t written = fwrite(data.items, sizeof(char), data.count, file);
  fclose(file);
  return written == data.count;
}

HELPER_FN bool helpers_copy_file(const char *src_path, const char *dst_path) {
  FILE *src = fopen(src_path, "rb");
  if (!src) {
    return false;
  }
  FILE *dst = fopen(dst_path, "wb");
  if (!dst) {
    fclose(src);
    return false;
  }
  char buffer[8192];
  size_t bytes;
  while ((bytes = fread(buffer, sizeof(char), sizeof(buffer), src)) > 0) {
    fwrite(buffer, sizeof(char), bytes, dst);
  }
  fclose(src);
  fclose(dst);
  return true;
}

HELPER_FN HelperStringView nob_temp_file_ext(const char *path) {
#ifndef _WIN32
  if (!path)
    path = ""; // Treating NULL as empty.
  const char *ext = strrchr(path, '.');
  if (ext) {
    return helpers_str_view_from_parts(ext, strlen(ext));
  } else {
    return helpers_str_view_from_parts("", 0);
  }
#else
  if (!path)
    path = ""; // Treating NULL as empty.
  char *ext = (char *)helpers_temp_alloc(_MAX_EXT);
  // https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/8e46eyt7(v=vs.100)
  errno_t ret = _splitpath_s(path, NULL, 0, NULL, 0, NULL, 0, ext, _MAX_EXT);
  HELPER_ASSERT(ret == 0);
  return helpers_str_view_from_parts(ext, strlen(ext));
#endif // _WIN32
}
#endif /* HELPERS_IMPLEMENTATION */

#define HELPERS_STRIP_PREFIX
#ifdef HELPERS_STRIP_PREFIX
#define arr_reserve helpers_arr_reserve
#define arr_append helpers_arr_append
#define arr_multiappend helpers_arr_multiappend
#define arr_free helpers_arr_free
#define arr_size helpers_arr_size
#define arr_capacity helpers_arr_capacity
#define String HelperString
#define StringView HelperStringView
#define string_from_cstr helpers_string_from_cstr
#define read_file helpers_read_file
#define str_trim_left helpers_str_trim_left
#define str_trim_right helpers_str_trim_right
#define str_trim helpers_str_trim
#define str_view_eq helpers_str_view_eq
#define str_view_eq_cstr helpers_str_view_eq_cstr
#define str_view_starts_with helpers_str_view_starts_with
#define str_view_ends_with helpers_str_view_ends_with
#define file_exists helpers_file_exists
#define dir_exists helpers_dir_exists
#define create_dir helpers_create_dir
#define write_file helpers_write_file
#define copy_file helpers_copy_file
#define temp_file_ext nob_temp_file_ext
#endif

#endif /* helpers_h */
