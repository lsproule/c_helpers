#define HELPERS_STRIP_PREFIX
#include "helpers.h"


int main() {
    struct {
        int *items;
        size_t count;
        size_t capacity;
    } arr = {0};

    for (int i = 0; i < 10; i++) {
        arr_append(&arr, i);
    }

    for (size_t i = 0; i < arr_size(arr); i++) {
        printf("%d ", arr.items[i]);
    }
    printf("\n");

    arr_free(arr);

    return 0;
}
