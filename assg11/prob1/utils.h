#ifndef UTILS_H
#define UTILS_H

#include "graph.h"

// Main Functions
void handle_queries(Graph *graph, int long_output);

// Helper Functions
int prompt_for_query(char **query);
void process_query(Graph *graph, const char *query, int long_output);
void process_actor_query(Graph *graph, ActorNode *actor, const char *query, int long_output);
void trim_newline(char *str);
ActorNode *find_actor_for_query(Graph *graph, const char *query);
void print_bacon_number(const char *query, int bacon_number);
void print_path(Graph *graph, const char *query);
void print_actor_path(char **actor_chain, char **movie_chain, int chain_length);
void free_actor_and_movie_chains(char **actor_chain, char **movie_chain, int chain_length);

#endif

