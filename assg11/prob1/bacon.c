/*
 * File: bacon.c
 * Author: Matthew W. Song
 * Purpose: Implements the main program logic for calculating Bacon numbers
 * and handling user queries interactively.
 */

#include "bfs.h"
#include "file_parser.h"
#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * free_file_lines(lines, num_lines) -- Frees memory allocated for an array of
 * lines.
 *
 * Arguments:
 *   lines: Pointer to the array of strings (lines).
 *   num_lines: Number of lines in the array.
 */
void free_file_lines(char **lines, int num_lines) {
  for (int i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);
}

/*
 * bacon_program(argc, argv) -- Core program logic for calculating Bacon
 * numbers. Parses command-line arguments, reads input files, builds the actor
 * graph, initializes BFS, and handles user queries interactively.
 *
 * Arguments:
 *   argc: The number of command-line arguments.
 *   argv: The array of command-line arguments.
 *
 * Assumptions:
 *   - The input file follows the specified format of movie entries.
 *   - The program is invoked with the correct arguments.
 *
 * Exits:
 *   - Exits with status 1 if any error occurs (e.g., invalid input file).
 *   - Exits with status 0 upon successful execution.
 */
void bacon_program(int argc, char *argv[]) {
  int long_output = 0;   // Flag for the -l option
  char *filename = NULL; // Input file name

  // Parse command-line arguments
  if (parse_arguments(argc, argv, &long_output, &filename) < 0) {
    fprintf(stderr, "Usage: %s [-l] inFile\n", argv[0]);
    exit(1);
  }

  // Read and parse the input file
  int num_lines = 0;
  char **lines = read_lines(filename, &num_lines);
  if (!lines) {
    fprintf(stderr, "Error: Unable to read file %s\n", filename);
    exit(1);
  }

  MovieEntry *movie_list = parse_movie_entries(lines, num_lines);
  if (!movie_list) {
    fprintf(stderr, "Error: Unable to parse movie entries from %s\n", filename);
    free_file_lines(lines, num_lines);
    exit(1);
  }
  free_file_lines(lines, num_lines);

  // Build the graph
  Graph *graph = build_actor_graph(movie_list);
  if (!graph) {
    fprintf(stderr, "Error: Unable to build graph from movie entries.\n");
    free_movie_entry_list(movie_list);
    exit(1);
  }
  free_movie_entry_list(movie_list);

  // Initialize BFS with Kevin Bacon as the start node
  if (init_bfs(graph, "Kevin Bacon") < 0) {
    fprintf(stderr, "Error: Kevin Bacon not found in the graph.\n");
    free_graph(graph);
    exit(1);
  }

  // Handle user queries interactively
  printf("Graph successfully built from %s.\n", filename);
  printf("Enter actor names to calculate their Bacon number.\n");
  printf("Type Ctrl+D (EOF) to quit.\n\n");
  handle_queries(graph, long_output);

  // Cleanup
  free_graph(graph);
}

/*
 * main(argc, argv) -- Entry point of the program.
 * Delegates execution to bacon_program.
 *
 * Arguments:
 *   argc: The number of command-line arguments.
 *   argv: The array of command-line arguments.
 *
 * Returns:
 *   0 on successful execution, 1 if an error occurs.
 */
int main(int argc, char *argv[]) {
  bacon_program(argc, argv);
  return 0;
}
