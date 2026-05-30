#define HELPERS_STRIP_PREFIX
#define HELPERS_IMPLEMENTATION
#include "../../helpers.h"

int main() {

  char* nombre = "algo";



  if (!rmdir("tests/dir/test", .recursive = true)) {
    printf("Failed to read file\n");
    return 1;
  }

  printf("Directory removed successfully\n");

  mkdir("tests/dir/test");
  mkdir("tests/dir/test/algo/nose", .recursive = true);

  StringView sv;
  sv.items = (char *)"file1.txt";
  sv.count = strlen(sv.items);

  write_file("tests/dir/test/file1.txt", sv);
  return 0;
}
