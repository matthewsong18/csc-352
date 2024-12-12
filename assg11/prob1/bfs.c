#include "bfs.h"
#include "graph.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main Functions

int init_bfs(Graph *g, const char *start_actor_name) {
  ActorNode *start = find_actor(g, start_actor_name);
  if (!start)
    return log_error("Starting actor not found.");

  initialize_bfs_state(g, start);
  Queue *queue = init_queue();
  if (!queue)
    return log_error("Failed to initialize queue.");

  enqueue(queue, start);
  int result = run_bfs(g, queue);
  free_queue(queue);
  return result;
}

int run_bfs(Graph *g, Queue *queue) {
  ActorNode *current = dequeue(queue);
  while (current) {
    visit_actor_edges(current, queue);
    current = dequeue(queue);
  }
  return 0;
}

int get_bacon_score(Graph *g, const char *actor_name) {
  ActorNode *actor = find_actor(g, actor_name);
  return actor ? actor->distance : -1;
}

int reconstruct_path(Graph *graph, const char *actor_name, char ***actor_chain,
                     char ***movie_chain, int *chain_length) {
  ActorNode *target = find_actor_in_graph(graph, actor_name);
  if (!target || target->distance == -1)
    return -1;

  int steps = count_path_steps(target);
  *chain_length = steps;
  return build_path_arrays(target, steps, actor_chain, movie_chain);
}

// Helper Functions

void initialize_bfs_state(Graph *g, ActorNode *start) {
  for (ActorNode *a = g->actors; a; a = a->next) {
    a->distance = -1;
    a->parent = NULL;
  }
  start->distance = 0;
}

void visit_actor_edges(ActorNode *current, Queue *queue) {
  ActorEdge *edge = current->edges;
  while (edge) {
    ActorNode *neighbor = edge->actor;
    if (neighbor->distance == -1) {
      neighbor->distance = current->distance + 1;
      neighbor->parent = current;
      enqueue(queue, neighbor);
    }
    edge = edge->next;
  }
}

ActorNode *find_actor_in_graph(Graph *graph, const char *actor_name) {
  return find_actor(graph, actor_name);
}

int count_path_steps(ActorNode *target) {
  int steps = 0;
  while (target && target->distance != 0) {
    steps++;
    target = target->parent;
  }
  return steps;
}

int build_path_arrays(ActorNode *target, int steps, char ***actor_chain,
                      char ***movie_chain) {
  if (!allocate_path_arrays(actor_chain, movie_chain, steps))
    return -1;
  fill_path_arrays(target, steps, *actor_chain, *movie_chain);
  return 0;
}

int allocate_path_arrays(char ***actor_chain, char ***movie_chain, int steps) {
  *actor_chain = malloc((steps + 1) * sizeof(char *));
  *movie_chain = malloc(steps * sizeof(char *));
  if (!*actor_chain || !*movie_chain) {
    free(*actor_chain);
    free(*movie_chain);
    return 0;
  }
  return 1;
}

void fill_path_arrays(ActorNode *target, int steps, char **actor_chain,
                      char **movie_chain) {
  int index = steps;
  actor_chain[index] = strdup(target->name);
  while (target && target->distance != 0) {
    ActorNode *parent = target->parent;
    index--;
    movie_chain[index] = strdup(get_connecting_movie(parent, target));
    actor_chain[index] = strdup(parent->name);
    target = parent;
  }
}

char *get_connecting_movie(ActorNode *from, ActorNode *to) {
  ActorEdge *edge = from->edges;
  while (edge) {
    if (edge->actor == to)
      return edge->movie;
    edge = edge->next;
  }
  return NULL;
}

int log_error(const char *message) {
  fprintf(stderr, "Error: %s\n", message);
  return -1;
}
