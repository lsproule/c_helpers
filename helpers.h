#ifndef helpers_h
#define helpers_h
#include <string.h>
#include <stdio.h>

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


typedef struct {
  size_t count;
  size_t capacity;
  char* items;
} HelperString;

typedef struct {
  size_t count;
  const char* items;
} HelperStringView;




HELPER_FN HelperString helpers_string_from_cstr(const char*);
HELPER_FN bool helpers_read_file(const char* path, HelperString* out);

//#define HELPERS_IMPLEMENTATION
#ifdef HELPERS_IMPLEMENTATION
HELPER_FN HelperString helpers_string_from_cstr(const char* cstr) {
  HelperString str = {0};
  size_t len = strlen(cstr);
  helpers_arr_reserve(&str, len);
  memcpy(str.items, cstr, len);
  str.count = len;
  return str;
}

HELPER_FN bool helpers_read_file(const char* path, HelperString* out) {
  FILE* file = fopen(path, "rb");
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
#endif

//#define HELPERS_STRIP_PREFIX
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

#endif

#endif /* helpers_h */
