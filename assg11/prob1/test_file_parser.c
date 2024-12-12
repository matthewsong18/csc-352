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

// int test_check_movie_line() {
//   char *movie_line = "Movie: A Random Movie!";

//   assert(is_movie_line(movie_line) == 1);

//   return 0;
// }

int test_parse_arguments_no_flags() {
  // Simulate argv: ["bacon", "movies.txt"]
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

int test_parse_arguments_with_flag() {
  // Simulate argv: ["bacon", "-l", "movies.txt"]
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

int test_parse_arguments_flag_after_filename() {
  // Simulate argv: ["bacon", "movies.txt", "-l"]
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

int test_parse_arguments_multiple_l_flags() {
  // Simulate argv: ["bacon", "-l", "-l", "movies.txt"]
  char *argv[] = {"bacon", "-l", "-l", "movies.txt"};
  int argc = 4;
  int long_output = -1;
  char *filename = NULL;

  int result = parse_arguments(argc, argv, &long_output, &filename);
  assert(result == 0);
  assert(long_output ==
         1); // Multiple -l flags still just means long_output = 1
  assert(filename != NULL && strcmp(filename, "movies.txt") == 0);

  return 0;
}

int test_read_lines() {
  // Create a temporary file for testing
  const char *test_filename = "test_input.txt";
  FILE *f = fopen(test_filename, "w");
  assert(f != NULL);

  // Write some lines, including blank lines
  // We'll have 3 non-blank lines and 2 blank lines
  // Non-blank lines: "Line 1", "Line 3", "Last Line"
  fprintf(f, "Line 1\n");    // non-blank
  fprintf(f, "\n");          // blank
  fprintf(f, "Line 3   \n"); // non-blank (has trailing spaces but consider it
                             // non-blank)
  fprintf(f, "\n");          // blank
  fprintf(f, "Last Line\n"); // non-blank
  fclose(f);

  int num_lines = 0;
  char **lines = read_lines(test_filename, &num_lines);

  // We expect num_lines == 3
  assert(num_lines == 3);

  // Check the returned lines
  // Note: We said we wouldn't trim whitespace other than newline, so "Line 3 "
  // stays as is
  assert(strcmp(lines[0], "Line 1") == 0);
  assert(strcmp(lines[1], "Line 3   ") == 0);
  assert(strcmp(lines[2], "Last Line") == 0);

  // Free memory
  for (int i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);

  // Remove the temporary file
  remove(test_filename);

  return 0;
}

// Helper function to count actors in a linked list
static int count_actors(ActorNameNode *head) {
  int count = 0;
  for (ActorNameNode *n = head; n != NULL; n = n->next) {
    count++;
  }
  return count;
}

// Helper function to check if the actor names match the expected list
// expected_actors is a null-terminated array of strings for simplicity.
static void check_actors(ActorNameNode *head, const char *expected_actors[]) {
  ActorNameNode *current = head;
  int i = 0;
  while (expected_actors[i] != NULL) {
    assert(current != NULL); // Ensure there's an actor node
    assert(strcmp(current->name, expected_actors[i]) == 0);
    current = current->next;
    i++;
  }
  // Ensure there are no extra actors
  assert(current == NULL);
}

int test_parse_movie_entries() {
  // Test input lines
  // We'll have two movies:
  // Movie: The Big Hit
  // Actor One
  // Actor Two
  // Movie: Another Movie
  // Actor A
  // Actor B
  // Actor C
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

  // Check the first movie
  MovieEntry *m1 = movie_list;
  assert(strcmp(m1->title, "The Big Hit") == 0);

  // Expected actors for the first movie
  const char *expected_m1_actors[] = {"Actor One", "Actor Two", NULL};
  assert(count_actors(m1->actors) == 2);
  check_actors(m1->actors, expected_m1_actors);

  // Check the second movie
  MovieEntry *m2 = m1->next;
  assert(m2 != NULL);
  assert(strcmp(m2->title, "Another Movie") == 0);

  // Expected actors for the second movie
  const char *expected_m2_actors[] = {"Actor A", "Actor B", "Actor C", NULL};
  assert(count_actors(m2->actors) == 3);
  check_actors(m2->actors, expected_m2_actors);

  // Ensure no further movies
  assert(m2->next == NULL);

  // Cleanup the linked lists
  // Free all allocated memory
  MovieEntry *m = movie_list;
  while (m) {
    MovieEntry *next_movie = m->next;
    free(m->title);
    ActorNameNode *a = m->actors;
    while (a) {
      ActorNameNode *next_actor = a->next;
      free(a->name);
      free(a);
      a = next_actor;
    }
    free(m);
    m = next_movie;
  }

  return 0;
}

int main(void) {
  test_blank_line();
  // test_check_movie_line();

  test_parse_arguments_no_flags();
  test_parse_arguments_with_flag();
  test_parse_arguments_flag_after_filename();
  test_parse_arguments_multiple_l_flags();

  test_read_lines();

  test_parse_movie_entries();

  printf("All file_parser tests passed!\n");
  fflush(stdout);
  return 0;
}
