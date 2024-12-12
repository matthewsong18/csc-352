#include "utils.h"
#include "bfs.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main Functions

void handle_queries(Graph *graph, int long_output) {
  char *query = NULL;
  while (prompt_for_query(&query)) {
    process_query(graph, query, long_output);
    free(query);  // Free memory after processing each query
    query = NULL; // Reset query to ensure it’s safe for getline
  }
}

int prompt_for_query(char **query) {
  printf("Enter actor name (or Ctrl+D to quit): ");
  *query = NULL; // Ensure query starts as NULL for getline
  size_t buffer_size = 0;
  ssize_t input_length = getline(query, &buffer_size, stdin);

  return input_length != -1; // Return 0 if EOF is encountered
}

void process_query(Graph *graph, const char *query, int long_output) {
  char *trimmed_query = strdup(query);
  trim_newline(trimmed_query);

  ActorNode *actor = find_actor_for_query(graph, trimmed_query);
  if (actor) {
    process_actor_query(graph, actor, trimmed_query, long_output);
  }

  free(trimmed_query);
}

void process_actor_query(Graph *graph, ActorNode *actor, const char *query,
                         int long_output) {
  int bacon_number = get_bacon_score(graph, query);
  print_bacon_number(query, bacon_number);

  if (long_output && bacon_number > 0) {
    print_path(graph, query);
  }
}

// Helper Functions

void trim_newline(char *str) {
  size_t length = strlen(str);
  if (length > 0 && str[length - 1] == '\n') {
    str[length - 1] = '\0';
  }
}

ActorNode *find_actor_for_query(Graph *graph, const char *query) {
  ActorNode *actor = find_actor(graph, query);
  if (!actor || actor->distance == -1) {
    printf("%s: unreachable\n", query);
    return NULL;
  }
  return actor;
}

void print_bacon_number(const char *query, int bacon_number) {
  printf("%s: Bacon number is %d\n", query, bacon_number);
}

void print_path(Graph *graph, const char *query) {
  char **actor_chain = NULL, **movie_chain = NULL;
  int chain_length = 0;

  if (reconstruct_path(graph, query, &actor_chain, &movie_chain,
                       &chain_length) == 0) {
    print_actor_path(actor_chain, movie_chain, chain_length);
    free_actor_and_movie_chains(actor_chain, movie_chain, chain_length);
  }
}

void print_actor_path(char **actor_chain, char **movie_chain,
                      int chain_length) {
  printf("Path: ");
  for (int i = 0; i < chain_length; i++) {
    printf("%s -> (%s) -> ", actor_chain[i], movie_chain[i]);
  }
  printf("%s\n", actor_chain[chain_length]);
}

void free_actor_and_movie_chains(char **actor_chain, char **movie_chain,
                                 int chain_length) {
  for (int i = 0; i <= chain_length; i++) {
    free(actor_chain[i]);
  }
  for (int i = 0; i < chain_length; i++) {
    free(movie_chain[i]);
  }
  free(actor_chain);
  free(movie_chain);
}
