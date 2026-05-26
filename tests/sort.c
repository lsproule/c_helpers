#define HELPERS_STRIP_PREFIX
#define HELPERS_IMPLEMENTATION
#include "../helpers.h"

typedef struct {
  char *nombre;
  int edad;
} Pingu;

typedef struct {
  Pingu *items;
  size_t count;
  size_t capacity;
} PinguArray;

int pingu_compare(const void *a, const void *b) {
  const Pingu *pa = (const Pingu *)a;
  const Pingu *pb = (const Pingu *)b;
  return (pa->edad > pb->edad) - (pa->edad < pb->edad);
}

int main() {
  struct {
    int *items;
    size_t count;
    size_t capacity;
  } arr_int = {0};

  struct {
    float *items;
    size_t count;
    size_t capacity;
  } arr_float = {0};

  struct {
    String *items;
    size_t count;
    size_t capacity;
  } arr_string = {0};

  PinguArray arr_pingu = {0};

  for (int i = 0; i < 10; i++) {
    int rand_num = rand() % 100;

    arr_append(&arr_int, i + rand_num);
    arr_append(&arr_float, (float)(i + rand_num) / 10.1f);

    char buffer[20];
    snprintf(buffer, sizeof(buffer), "penguino: %d", i + rand_num);
    snprintf(buffer, sizeof(buffer), "penguino: %d", i + rand_num);

    Pingu pingu = {0};
    pingu.edad = i + rand_num;
    pingu.nombre = malloc(20);
    snprintf(pingu.nombre, sizeof(pingu.nombre[0]) * 20, "penguino: %d",
             i + rand_num);

    String str = string_from_cstr(buffer);

    arr_append(&arr_string, str);

    arr_append(&arr_pingu, pingu);
  }

  merge_sort(arr_int.items, arr_int.count);
  merge_sort(arr_float.items, arr_float.count);
  merge_sort(arr_string.items, arr_string.count);


  //custom type
  merge_sort(arr_pingu.items, arr_pingu.count, .compare = pingu_compare);

  for (size_t i = 0; i < arr_size(arr_int); i++) {
    printf("%d ", arr_int.items[i]);
  }
  printf("\n");

  for (size_t i = 0; i < arr_size(arr_float); i++) {
    printf("%.2f ", arr_float.items[i]);
  }
  printf("\n");

  for (size_t i = 0; i < arr_size(arr_string); i++) {
    printf("%s ", arr_string.items[i].items);
  }

  arr_free(arr_int);
  arr_free(arr_float);
  arr_free(arr_string);

  return 0;
}
