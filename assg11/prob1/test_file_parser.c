#include "assert.h"
#include "file_parser.h"
#include <stdio.h>

int test_blank_line() {
  char *null_line = NULL;
  char *empty_line = "";
  char *white_space_line = " ";
  char *non_empty_line = "Not empty";

  assert(is_blank_line(null_line) == 1);
  assert(is_blank_line(empty_line) == 1);
  assert(is_blank_line(white_space_line) == 1);
  assert(is_blank_line(non_empty_line) == 0);

  return 0;
}

int main(void) {
  test_blank_line();

  printf("All file_parser tests passed!");
  fflush(stdout);
  return 0;
}
