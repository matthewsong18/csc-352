#include "assert.h"
#include "file_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * test_blank_line() -- Tests the behavior of is_non_blank_line() with different
 * input cases.
 */
int test_blank_line() {
  char *null_line = NULL;
  char *empty_line = "";
  char *white_space_line = " ";
  char *non_empty_line = "Not empty";

  assert(is_non_blank_line(null_line) == 0);
  assert(is_non_blank_line(empty_line) == 0);
  assert(is_non_blank_line(white_space_line) == 0);
  assert(is_non_blank_line(non_empty_line) == 1);

  return 0;
}

/*
 * test_parse_arguments_no_flags() -- Tests parse_arguments() with no flags,
 * only a filename.
 */
int test_parse_arguments_no_flags() {
  char *argv[] = {"bacon", "movies.txt"};
  int argc = 2;
  int long_output = -1;
  char *filename = NULL;

  int result = parse_arguments(argc, argv, &long_output, &filename);
  assert(result == 0);
  assert(long_output == 0);
  assert(filename != NULL && strcmp(filename, "movies.txt") == 0);

  return 0;
}

/*
 * test_parse_arguments_with_flag() -- Tests parse_arguments() with the -l flag
 * before the filename.
 */
int test_parse_arguments_with_flag() {
  char *argv[] = {"bacon", "-l", "movies.txt"};
  int argc = 3;
  int long_output = -1;
  char *filename = NULL;

  int result = parse_arguments(argc, argv, &long_output, &filename);
  assert(result == 0);
  assert(long_output == 1);
  assert(filename != NULL && strcmp(filename, "movies.txt") == 0);

  return 0;
}

/*
 * test_parse_arguments_flag_after_filename() -- Tests parse_arguments() with
 * the -l flag after the filename.
 */
int test_parse_arguments_flag_after_filename() {
  char *argv[] = {"bacon", "movies.txt", "-l"};
  int argc = 3;
  int long_output = -1;
  char *filename = NULL;

  int result = parse_arguments(argc, argv, &long_output, &filename);
  assert(result == 0);
  assert(long_output == 1);
  assert(filename != NULL && strcmp(filename, "movies.txt") == 0);

  return 0;
}

/*
 * test_parse_arguments_multiple_l_flags() -- Tests parse_arguments() with
 * multiple -l flags.
 */
int test_parse_arguments_multiple_l_flags() {
  char *argv[] = {"bacon", "-l", "-l", "movies.txt"};
  int argc = 4;
  int long_output = -1;
  char *filename = NULL;

  int result = parse_arguments(argc, argv, &long_output, &filename);
  assert(result == 0);
  assert(long_output == 1);
  assert(filename != NULL && strcmp(filename, "movies.txt") == 0);

  return 0;
}

/*
 * test_read_lines() -- Tests read_lines() to correctly read and process
 * non-blank lines.
 */
int test_read_lines() {
  const char *test_filename = "test_input.txt";
  FILE *f = fopen(test_filename, "w");
  assert(f != NULL);

  fprintf(f, "Line 1\n");
  fprintf(f, "\n");
  fprintf(f, "Line 3   \n");
  fprintf(f, "\n");
  fprintf(f, "Last Line\n");
  fclose(f);

  int num_lines = 0;
  char **lines = read_lines(test_filename, &num_lines);

  assert(num_lines == 3);
  assert(strcmp(lines[0], "Line 1") == 0);
  assert(strcmp(lines[1], "Line 3   ") == 0);
  assert(strcmp(lines[2], "Last Line") == 0);

  for (int i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);

  remove(test_filename);
  return 0;
}

/*
 * Helper function to count actors in a linked list.
 */
static int count_actors(ActorNameNode *head) {
  int count = 0;
  for (ActorNameNode *n = head; n != NULL; n = n->next) {
    count++;
  }
  return count;
}

/*
 * Helper function to validate actor names against an expected list.
 */
static void check_actors(ActorNameNode *head, const char *expected_actors[]) {
  ActorNameNode *current = head;
  int i = 0;
  while (expected_actors[i] != NULL) {
    assert(current != NULL);
    assert(strcmp(current->name, expected_actors[i]) == 0);
    current = current->next;
    i++;
  }
  assert(current == NULL);
}

/*
 * test_parse_movie_entries() -- Tests parse_movie_entries() for correct parsing
 * of movies and actors.
 */
int test_parse_movie_entries() {
  char *lines[] = {"Movie: The Big Hit",
                   "Actor One",
                   "Actor Two",
                   "Movie: Another Movie",
                   "Actor A",
                   "Actor B",
                   "Actor C"};
  int num_lines = 7;

  MovieEntry *movie_list = parse_movie_entries(lines, num_lines);
  assert(movie_list != NULL);

  MovieEntry *m1 = movie_list;
  assert(strcmp(m1->title, "The Big Hit") == 0);

  const char *expected_m1_actors[] = {"Actor One", "Actor Two", NULL};
  assert(count_actors(m1->actors) == 2);
  check_actors(m1->actors, expected_m1_actors);

  MovieEntry *m2 = m1->next;
  assert(m2 != NULL);
  assert(strcmp(m2->title, "Another Movie") == 0);

  const char *expected_m2_actors[] = {"Actor A", "Actor B", "Actor C", NULL};
  assert(count_actors(m2->actors) == 3);
  check_actors(m2->actors, expected_m2_actors);

  assert(m2->next == NULL);

  free_movie_entry_list(movie_list);
  return 0;
}

/*
 * main() -- Entry point for the test suite.
 */
int main(void) {
  test_blank_line();
  test_parse_arguments_no_flags();
  test_parse_arguments_with_flag();
  test_parse_arguments_flag_after_filename();
  test_parse_arguments_multiple_l_flags();
  test_read_lines();
  test_parse_movie_entries();

  printf("All file_parser tests passed!\n");
  return 0;
}
