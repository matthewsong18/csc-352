#include "bfs.h"
#include "file_parser.h"
#include "graph.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Ensure bacon_program is accessible
extern void bacon_program(int argc, char *argv[]);

// Helper to create a mock input file for testing
static void create_mock_input_file(const char *filename) {
  FILE *file = fopen(filename, "w");
  assert(file != NULL);

  fprintf(file, "Movie: Apollo 13\n"
                "Kevin Bacon\n"
                "Ed Harris\n"
                "Movie: Footloose\n"
                "Kevin Bacon\n"
                "Lori Singer\n"
                "Movie: The Rock\n"
                "Ed Harris\n"
                "Sean Connery\n");

  fclose(file);
}

// Helper to capture the output of bacon_program
static char *capture_output(int argc, char *argv[]) {
  int pipe_fd[2];
  assert(pipe(pipe_fd) == 0);

  // Redirect stdout to the write end of the pipe
  int saved_stdout = dup(STDOUT_FILENO);
  dup2(pipe_fd[1], STDOUT_FILENO);
  close(pipe_fd[1]);

  // Run the bacon_program
  bacon_program(argc, argv);

  // Restore stdout
  fflush(stdout);
  dup2(saved_stdout, STDOUT_FILENO);
  close(saved_stdout);

  // Read from the pipe
  char *output = malloc(4096);
  int bytes_read = read(pipe_fd[0], output, 4095);
  close(pipe_fd[0]);
  if (bytes_read >= 0) {
    output[bytes_read] = '\0';
  } else {
    output[0] = '\0';
  }
  return output;
}

// Test case for bacon_program
void test_bacon() {
  // Create a mock input file
  const char *mock_file = "mock_movies.txt";
  create_mock_input_file(mock_file);

  // Pass the mock file as an argument to bacon_program
  char *argv[] = {"bacon", "-l", (char *)mock_file};
  char *output = capture_output(3, argv);

  // Validate the output
  assert(strstr(output, "Enter actor name") != NULL);
  assert(strstr(output, "Kevin Bacon") ==
         NULL); // Kevin Bacon isn't queried automatically
  printf("Captured Output:\n%s\n", output);

  // Free captured output
  free(output);

  // Clean up mock file
  remove(mock_file);

  printf("test_bacon passed!\n");
}

int main(void) {
  test_bacon();
  printf("All tests passed!\n");
  return 0;
}
