/*
 * File: bfs.c
 * Author: Matthew W. Song
 * Purpose: Implements Breadth-First Search (BFS) logic for graph traversal,
 * calculating Bacon numbers, and reconstructing paths.
 */

#include "bfs.h"
#include "graph.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * init_bfs(g, start_actor_name) -- Initializes BFS on the graph starting
 * from the specified actor. Sets up distances, parents, and enqueues the
 * start actor for BFS traversal.
 *
 * Arguments:
 *   g: Pointer to the graph.
 *   start_actor_name: Name of the actor to start the BFS.
 *
 * Returns:
 *   0 on success, -1 if the start actor is not found or initialization fails.
 */
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

/*
 * run_bfs(g, queue) -- Executes BFS traversal on the graph using the provided
 * queue.
 *
 * Arguments:
 *   g: Pointer to the graph.
 *   queue: Pointer to the initialized queue.
 *
 * Returns:
 *   0 on successful traversal.
 */
int run_bfs(Graph *g, Queue *queue) {
  ActorNode *current = dequeue(queue);
  while (current) {
    visit_actor_edges(current, queue);
    current = dequeue(queue);
  }
  return 0;
}

/*
 * get_bacon_score(g, actor_name) -- Retrieves the Bacon number of the specified
 * actor.
 *
 * Arguments:
 *   g: Pointer to the graph.
 *   actor_name: Name of the actor whose Bacon number is required.
 *
 * Returns:
 *   Bacon number if the actor is reachable, -1 otherwise.
 */
int get_bacon_score(Graph *g, const char *actor_name) {
  ActorNode *actor = find_actor(g, actor_name);
  return actor ? actor->distance : -1;
}

/*
 * reconstruct_path(graph, actor_name, actor_chain, movie_chain, chain_length)
 * -- Reconstructs the path from original to the specified actor.
 *
 * Arguments:
 *   graph: Pointer to the graph.
 *   actor_name: Name of the actor.
 *   actor_chain: Pointer to an array of actor names.
 *   movie_chain: Pointer to an array of movie titles.
 *   chain_length: Pointer to store the length of the path.
 *
 * Returns:
 *   0 on success, -1 if the actor is unreachable or memory allocation fails.
 */
int reconstruct_path(Graph *graph, const char *actor_name, char ***actor_chain,
                     char ***movie_chain, int *chain_length) {
  ActorNode *target = find_actor_in_graph(graph, actor_name);
  if (!target || target->distance == -1)
    return -1;

  int steps = count_path_steps(target);
  *chain_length = steps;
  return build_path_arrays(target, steps, actor_chain, movie_chain);
}

/*
 * initialize_bfs_state(g, start) -- Resets distances and parents for all nodes
 * and sets the starting node distance to 0.
 *
 * Arguments:
 *   g: Pointer to the graph.
 *   start: Pointer to the starting ActorNode.
 */
void initialize_bfs_state(Graph *g, ActorNode *start) {
  for (ActorNode *a = g->actors; a; a = a->next) {
    a->distance = -1;
    a->parent = NULL;
  }
  start->distance = 0;
}

/*
 * visit_actor_edges(current, queue) -- Processes the edges of the current
 * actor, updates distances and parents, and enqueues neighbors.
 *
 * Arguments:
 *   current: Pointer to the current ActorNode.
 *   queue: Pointer to the queue.
 */
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

/*
 * find_actor_in_graph(graph, actor_name) -- Finds an actor in the graph by
 * name.
 *
 * Arguments:
 *   graph: Pointer to the graph.
 *   actor_name: Name of the actor to find.
 *
 * Returns:
 *   Pointer to the ActorNode if found, NULL otherwise.
 */
ActorNode *find_actor_in_graph(Graph *graph, const char *actor_name) {
  return find_actor(graph, actor_name);
}

/*
 * count_path_steps(target) -- Counts the number of steps from the target actor
 * to the starting actor.
 *
 * Arguments:
 *   target: Pointer to the target ActorNode.
 *
 * Returns:
 *   Number of steps in the path.
 */
int count_path_steps(ActorNode *target) {
  int steps = 0;
  while (target && target->distance != 0) {
    steps++;
    target = target->parent;
  }
  return steps;
}

/*
 * build_path_arrays(target, steps, actor_chain, movie_chain) -- Allocates and
 * fills arrays for the actor and movie chains.
 *
 * Arguments:
 *   target: Pointer to the target ActorNode.
 *   steps: Number of steps in the path.
 *   actor_chain: Pointer to an array of actor names.
 *   movie_chain: Pointer to an array of movie titles.
 *
 * Returns:
 *   0 on success, -1 if memory allocation fails.
 */
int build_path_arrays(ActorNode *target, int steps, char ***actor_chain,
                      char ***movie_chain) {
  if (!allocate_path_arrays(actor_chain, movie_chain, steps))
    return -1;
  fill_path_arrays(target, steps, *actor_chain, *movie_chain);
  return 0;
}

/*
 * allocate_path_arrays(actor_chain, movie_chain, steps) -- Allocates memory for
 * the actor and movie chain arrays.
 *
 * Arguments:
 *   actor_chain: Pointer to an array of actor names.
 *   movie_chain: Pointer to an array of movie titles.
 *   steps: Number of steps in the path.
 *
 * Returns:
 *   1 on success, 0 if memory allocation fails.
 */
int allocate_path_arrays(char ***actor_chain, char ***movie_chain, int steps) {
  *actor_chain = malloc((steps + 1) * sizeof(char *));
  if (actor_chain == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  *movie_chain = malloc(steps * sizeof(char *));
  if (movie_chain == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!*actor_chain || !*movie_chain) {
    free(*actor_chain);
    free(*movie_chain);
    return 0;
  }
  return 1;
}

/*
 * fill_path_arrays(target, steps, actor_chain, movie_chain) -- Fills actor and
 * movie chains with names and titles from the BFS path.
 *
 * Arguments:
 *   target: Pointer to the target ActorNode.
 *   steps: Number of steps in the path.
 *   actor_chain: Array of actor names.
 *   movie_chain: Array of movie titles.
 */
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

/*
 * get_connecting_movie(from, to) -- Finds the movie connecting two actors.
 *
 * Arguments:
 *   from: Pointer to the first ActorNode.
 *   to: Pointer to the second ActorNode.
 *
 * Returns:
 *   Name of the connecting movie, or NULL if no connection exists.
 */
char *get_connecting_movie(ActorNode *from, ActorNode *to) {
  ActorEdge *edge = from->edges;
  while (edge) {
    if (edge->actor == to)
      return edge->movie;
    edge = edge->next;
  }
  return NULL;
}

/*
 * log_error(message) -- Logs an error message to stderr.
 *
 * Arguments:
 *   message: The error message to log.
 *
 * Returns:
 *   -1 as an error indicator.
 */
int log_error(const char *message) {
  fprintf(stderr, "Error: %s\n", message);
  return -1;
}
