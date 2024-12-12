#ifndef BFS_H
#define BFS_H

#include "graph.h"
#include "queue.h"

int init_bfs(Graph *g, const char *start_actor_name);
int run_bfs(Graph *g, Queue *queue);
int get_bacon_score(Graph *g, const char *actor_name);
int reconstruct_path(Graph *graph, const char *actor_name, char ***actor_chain, char ***movie_chain, int *chain_length);

void initialize_bfs_state(Graph *g, ActorNode *start);
void visit_actor_edges(ActorNode *current, Queue *queue);

ActorNode *find_actor_in_graph(Graph *graph, const char *actor_name);

int count_path_steps(ActorNode *target);
int build_path_arrays(ActorNode *target, int steps, char ***actor_chain, char ***movie_chain);

int allocate_path_arrays(char ***actor_chain, char ***movie_chain, int steps);
void fill_path_arrays(ActorNode *target, int steps, char **actor_chain, char **movie_chain);

char *get_connecting_movie(ActorNode *from, ActorNode *to);
int log_error(const char *message);

#endif

