/*
 * File: utils.c
 * Author: Matthew W. Song
 * Purpose: Implements utility functions for handling user queries,
 *          including prompting, processing, and outputting results
 *          for Bacon number calculations.
 */

#include "utils.h"
#include "bfs.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main Functions

/*
 * handle_queries(graph, long_output)
 * Main function to interactively handle user queries for Bacon numbers.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure containing actor data.
 *   long_output - Flag indicating whether to print the full path or just the
 * Bacon number.
 */
void handle_queries(Graph *graph, int long_output) {
  char *query = NULL;
  while (prompt_for_query(&query)) {
    process_query(graph, query, long_output);
    free(query);  // Free memory after processing each query
    query = NULL; // Reset query to ensure it’s safe for getline
  }
}

/*
 * prompt_for_query(query)
 * Prompts the user to enter an actor's name and reads the input.
 *
 * Arguments:
 *   query - Pointer to a string that will store the user's input.
 *
 * Returns:
 *   1 if input is successfully read, 0 if EOF is encountered.
 */
int prompt_for_query(char **query) {
  printf("Enter actor name (or Ctrl+D to quit): ");
  *query = NULL; // Ensure query starts as NULL for getline
  size_t buffer_size = 0;
  ssize_t input_length = getline(query, &buffer_size, stdin);

  return input_length != -1; // Return 0 if EOF is encountered
}

/*
 * process_query(graph, query, long_output)
 * Processes a single query by finding the actor, calculating the Bacon number,
 * and optionally printing the path.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure containing actor data.
 *   query - User input representing the actor's name.
 *   long_output - Flag indicating whether to print the full path.
 */
void process_query(Graph *graph, const char *query, int long_output) {
  char *trimmed_query = strdup(query);
  trim_newline(trimmed_query);

  ActorNode *actor = find_actor_for_query(graph, trimmed_query);
  if (actor) {
    process_actor_query(graph, actor, trimmed_query, long_output);
  }

  free(trimmed_query);
}

/*
 * process_actor_query(graph, actor, query, long_output)
 * Processes a query for a valid actor by calculating and printing the Bacon
 * number and optionally the full path.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure containing actor data.
 *   actor - Pointer to the ActorNode being queried.
 *   query - User input representing the actor's name.
 *   long_output - Flag indicating whether to print the full path.
 */
void process_actor_query(Graph *graph, ActorNode *actor, const char *query,
                         int long_output) {
  int bacon_number = get_bacon_score(graph, query);
  print_bacon_number(query, bacon_number);

  if (long_output && bacon_number > 0) {
    print_path(graph, query);
  }
}

// Helper Functions

/*
 * trim_newline(str)
 * Removes the trailing newline character from a string if present.
 *
 * Arguments:
 *   str - Pointer to the string to trim.
 */
void trim_newline(char *str) {
  size_t length = strlen(str);
  if (length > 0 && str[length - 1] == '\n') {
    str[length - 1] = '\0';
  }
}

/*
 * find_actor_for_query(graph, query)
 * Finds an actor in the graph based on the query and checks if they are
 * reachable.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure containing actor data.
 *   query - User input representing the actor's name.
 *
 * Returns:
 *   Pointer to the ActorNode if found and reachable, or NULL otherwise.
 */
ActorNode *find_actor_for_query(Graph *graph, const char *query) {
  ActorNode *actor = find_actor(graph, query);
  if (!actor || actor->distance == -1) {
    printf("%s: unreachable\n", query);
    return NULL;
  }
  return actor;
}

/*
 * print_bacon_number(query, bacon_number)
 * Prints the Bacon number of the queried actor.
 *
 * Arguments:
 *   query - User input representing the actor's name.
 *   bacon_number - The calculated Bacon number for the actor.
 */
void print_bacon_number(const char *query, int bacon_number) {
  printf("%s: Bacon number is %d\n", query, bacon_number);
}

/*
 * print_path(graph, query)
 * Prints the path of actors and movies that connects the queried actor to Kevin
 * Bacon.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure containing actor data.
 *   query - User input representing the actor's name.
 */
void print_path(Graph *graph, const char *query) {
  char **actor_chain = NULL, **movie_chain = NULL;
  int chain_length = 0;

  if (reconstruct_path(graph, query, &actor_chain, &movie_chain,
                       &chain_length) == 0) {
    print_actor_path(actor_chain, movie_chain, chain_length);
    free_actor_and_movie_chains(actor_chain, movie_chain, chain_length);
  }
}

/*
 * print_actor_path(actor_chain, movie_chain, chain_length)
 * Prints the sequence of actors and movies that form the connection path.
 *
 * Arguments:
 *   actor_chain - Array of actor names in the path.
 *   movie_chain - Array of movie titles connecting the actors.
 *   chain_length - Length of the connection path.
 */
void print_actor_path(char **actor_chain, char **movie_chain,
                      int chain_length) {
  printf("Path: ");
  for (int i = 0; i < chain_length; i++) {
    printf("%s -> (%s) -> ", actor_chain[i], movie_chain[i]);
  }
  printf("%s\n", actor_chain[chain_length]);
}

/*
 * free_actor_and_movie_chains(actor_chain, movie_chain, chain_length)
 * Frees memory allocated for actor and movie chains.
 *
 * Arguments:
 *   actor_chain - Array of actor names in the path.
 *   movie_chain - Array of movie titles connecting the actors.
 *   chain_length - Length of the connection path.
 */
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
