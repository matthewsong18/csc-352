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

int test_getting_movie_title() {
  char *movie_line = "Movie: A Random Movie!";

  assert(strcmp("A Random Movie!", get_movie_title(movie_line)) == 0);

  return 0;
}

int main(void) {
  test_blank_line();
  test_getting_movie_title();

  printf("All file_parser tests passed!");
  fflush(stdout);
  return 0;
}
