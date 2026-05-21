#define HELPERS_STRIP_PREFIX
#define HELPERS_IMPLEMENTATION
#include "../../helpers.h"

int main(){
    String file_data;
    if (!read_file("tests/file/file.txt", &file_data)) {
        printf("Failed to read file\n");
        return 1;
    }
    printf("File contents:\n%.*s\n", (int)file_data.count, file_data.items);
}
