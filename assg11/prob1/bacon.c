#include "bfs.h"
#include "file_parser.h"
#include "graph.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Core program logic moved here
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
    for (int i = 0; i < num_lines; i++)
      free(lines[i]);
    free(lines);
    exit(1);
  }

  for (int i = 0; i < num_lines; i++)
    free(lines[i]);
  free(lines);

  // Build the graph
  Graph *graph = build_actor_graph(movie_list);
  if (!graph) {
    fprintf(stderr, "Error: Unable to build graph from movie entries.\n");
    free_movie_list(movie_list);
    exit(1);
  }
  free_movie_list(movie_list);

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

// Simple main function for normal program execution
int main(int argc, char *argv[]) {
  bacon_program(argc, argv);
  return 0;
}
