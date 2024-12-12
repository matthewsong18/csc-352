/*
 * File: graph.c
 * Author: Matthew W. Song
 * Purpose: Implements functions for creating and managing a graph structure
 *          used to calculate Bacon numbers. This includes handling actors,
 *          movies, edges, and relationships between them.
 */

#include "graph.h"
#include "file_parser.h"
#include <stdlib.h>
#include <string.h>

// Main Functions

/*
 * init_graph()
 * Initializes and returns a new empty graph structure.
 *
 * Returns:
 *   Pointer to the initialized Graph structure, or NULL on failure.
 */
Graph *init_graph() { return create_empty_graph(); }

/*
 * build_actor_graph(movie_list)
 * Builds a graph from a linked list of MovieEntry structures.
 *
 * Arguments:
 *   movie_list - Pointer to the head of the MovieEntry linked list.
 *
 * Returns:
 *   Pointer to the constructed Graph, or NULL on failure.
 */
Graph *build_actor_graph(MovieEntry *movie_list) {
  Graph *graph = init_graph();
  if (!graph)
    return NULL;

  if (!populate_graph_from_movies(graph, movie_list)) {
    free_graph(graph);
    return NULL;
  }

  return graph;
}

/*
 * free_graph(graph)
 * Frees all memory associated with a Graph structure.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure to free.
 */
void free_graph(Graph *graph) {
  if (!graph)
    return;
  free_actor_list(graph->actors);
  free_movie_node_list(graph->movies);
  free(graph);
}

/*
 * find_actor(graph, actor_name)
 * Searches for an actor by name in the graph.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure.
 *   actor_name - Name of the actor to search for.
 *
 * Returns:
 *   Pointer to the ActorNode if found, or NULL otherwise.
 */
ActorNode *find_actor(Graph *graph, const char *actor_name) {
  return search_actor_list(graph->actors, actor_name);
}

/*
 * link_actors(actor1, actor2, movie_title)
 * Creates a bidirectional edge between two ActorNode structures.
 *
 * Arguments:
 *   actor1 - Pointer to the first ActorNode.
 *   actor2 - Pointer to the second ActorNode.
 *   movie_title - Title of the movie connecting the actors.
 */
void link_actors(ActorNode *actor1, ActorNode *actor2,
                 const char *movie_title) {
  create_bidirectional_edge(actor1, actor2, movie_title);
}

// Helper Functions for Graph Initialization

/*
 * create_empty_graph()
 * Allocates and initializes a new empty Graph structure.
 *
 * Returns:
 *   Pointer to the created Graph structure, or NULL on failure.
 */
Graph *create_empty_graph() {
  Graph *graph = malloc(sizeof(Graph));
  if (graph == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!graph)
    return NULL;

  graph->actors = NULL;
  graph->movies = NULL;
  return graph;
}

// Helper Functions for Graph Population

/*
 * populate_graph_from_movies(graph, movie_list)
 * Populates the graph with actors and their relationships based on movies.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure to populate.
 *   movie_list - Pointer to the MovieEntry linked list.
 *
 * Returns:
 *   1 on success, or 0 on failure.
 */
int populate_graph_from_movies(Graph *graph, MovieEntry *movie_list) {
  while (movie_list) {
    if (!process_single_movie(graph, movie_list))
      return 0;
    movie_list = movie_list->next;
  }
  return 1;
}

/*
 * process_single_movie(graph, movie)
 * Processes a single movie and links its actors in the graph.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure.
 *   movie - Pointer to the MovieEntry to process.
 *
 * Returns:
 *   1 on success, or 0 on failure.
 */
int process_single_movie(Graph *graph, MovieEntry *movie) {
  ActorNameNode *first_actor = movie->actors;
  while (first_actor) {
    if (!process_movie_actor(graph, first_actor, movie->title))
      return 0;
    first_actor = first_actor->next;
  }
  return 1;
}

/*
 * process_movie_actor(graph, actor_name_node, movie_title)
 * Processes a single actor in a movie, ensuring they are added to the graph
 * and linked to their co-actors.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure.
 *   actor_name_node - Pointer to the ActorNameNode structure.
 *   movie_title - Title of the movie.
 *
 * Returns:
 *   1 on success, or 0 on failure.
 */
int process_movie_actor(Graph *graph, ActorNameNode *actor_name_node,
                        const char *movie_title) {
  ActorNode *actor = get_or_create_actor(graph, actor_name_node->name);
  if (!actor)
    return 0;

  ActorNameNode *next_actor = actor_name_node->next;
  while (next_actor) {
    if (!link_movie_actors(graph, actor, next_actor, movie_title))
      return 0;
    next_actor = next_actor->next;
  }
  return 1;
}

/*
 * link_movie_actors(graph, actor1, actor2_name_node, movie_title)
 * Links two actors in the graph based on their shared movie.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure.
 *   actor1 - Pointer to the first ActorNode.
 *   actor2_name_node - Pointer to the ActorNameNode structure for the second
 * actor. movie_title - Title of the movie connecting the actors.
 *
 * Returns:
 *   1 on success, or 0 on failure.
 */
int link_movie_actors(Graph *graph, ActorNode *actor1,
                      ActorNameNode *actor2_name_node,
                      const char *movie_title) {
  ActorNode *actor2 = get_or_create_actor(graph, actor2_name_node->name);
  if (!actor2)
    return 0;

  link_actors(actor1, actor2, movie_title);
  return 1;
}

/*
 * get_or_create_actor(graph, actor_name)
 * Retrieves an existing ActorNode by name or creates a new one if it doesn't
 * exist.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure.
 *   actor_name - Name of the actor to retrieve or create.
 *
 * Returns:
 *   Pointer to the existing or newly created ActorNode, or NULL on failure.
 */
ActorNode *get_or_create_actor(Graph *graph, const char *actor_name) {
  ActorNode *existing_actor = find_actor(graph, actor_name);
  if (existing_actor)
    return existing_actor;

  return create_and_add_actor(graph, actor_name);
}

/*
 * create_and_add_actor(graph, actor_name)
 * Creates a new ActorNode and adds it to the graph's actor list.
 *
 * Arguments:
 *   graph - Pointer to the Graph structure.
 *   actor_name - Name of the actor to create.
 *
 * Returns:
 *   Pointer to the newly created ActorNode, or NULL on failure.
 */
ActorNode *create_and_add_actor(Graph *graph, const char *actor_name) {
  ActorNode *new_actor = create_actor_node(actor_name);
  if (!new_actor)
    return NULL;

  new_actor->next = graph->actors;
  graph->actors = new_actor;
  return new_actor;
}

// Helper Functions for Actor and Movie Lists

/*
 * free_actor_list(actor_list)
 * Frees the memory allocated for a linked list of ActorNode structures.
 *
 * Arguments:
 *   actor_list - Pointer to the head of the ActorNode linked list.
 */
void free_actor_list(ActorNode *actor_list) {
  while (actor_list) {
    ActorNode *next_actor = actor_list->next;
    free_actor(actor_list);
    actor_list = next_actor;
  }
}

/*
 * free_movie_node_list(movie_list)
 * Frees the memory allocated for a linked list of MovieNode structures.
 *
 * Arguments:
 *   movie_list - Pointer to the head of the MovieNode linked list.
 */
void free_movie_node_list(MovieNode *movie_list) {
  while (movie_list) {
    MovieNode *next_movie = movie_list->next;
    free_movie(movie_list);
    movie_list = next_movie;
  }
}

/*
 * search_actor_list(actor_list, actor_name)
 * Searches for an actor by name in a linked list of ActorNode structures.
 *
 * Arguments:
 *   actor_list - Pointer to the head of the ActorNode linked list.
 *   actor_name - Name of the actor to search for.
 *
 * Returns:
 *   Pointer to the ActorNode if found, or NULL otherwise.
 */
ActorNode *search_actor_list(ActorNode *actor_list, const char *actor_name) {
  while (actor_list) {
    if (strcmp(actor_list->name, actor_name) == 0)
      return actor_list;
    actor_list = actor_list->next;
  }
  return NULL;
}

// Helper Functions for Edges

/*
 * create_bidirectional_edge(actor1, actor2, movie_title)
 * Creates a bidirectional edge between two ActorNode structures.
 *
 * Arguments:
 *   actor1 - Pointer to the first ActorNode.
 *   actor2 - Pointer to the second ActorNode.
 *   movie_title - Title of the movie connecting the actors.
 */
void create_bidirectional_edge(ActorNode *actor1, ActorNode *actor2,
                               const char *movie_title) {
  create_edge(actor1, actor2, movie_title);
  create_edge(actor2, actor1, movie_title);
}

/*
 * create_edge(from_actor, to_actor, movie_title)
 * Creates a unidirectional edge from one ActorNode to another.
 *
 * Arguments:
 *   from_actor - Pointer to the source ActorNode.
 *   to_actor - Pointer to the destination ActorNode.
 *   movie_title - Title of the movie connecting the actors.
 */
void create_edge(ActorNode *from_actor, ActorNode *to_actor,
                 const char *movie_title) {
  ActorEdge *new_edge = malloc(sizeof(ActorEdge));
  if (new_edge == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!new_edge)
    return;

  new_edge->actor = to_actor;
  new_edge->movie = strdup_safe(movie_title);
  new_edge->next = from_actor->edges;
  from_actor->edges = new_edge;
}

// Helper Functions for Memory Management

/*
 * free_actor(actor)
 * Frees the memory allocated for a single ActorNode structure.
 *
 * Arguments:
 *   actor - Pointer to the ActorNode structure.
 */
void free_actor(ActorNode *actor) {
  free(actor->name);
  free_edge_list(actor->edges);
  free(actor);
}

/*
 * free_edge_list(edge_list)
 * Frees the memory allocated for a linked list of ActorEdge structures.
 *
 * Arguments:
 *   edge_list - Pointer to the head of the ActorEdge linked list.
 */
void free_edge_list(ActorEdge *edge_list) {
  while (edge_list) {
    ActorEdge *next_edge = edge_list->next;
    free_edge(edge_list);
    edge_list = next_edge;
  }
}

/*
 * free_edge(edge)
 * Frees the memory allocated for a single ActorEdge structure.
 *
 * Arguments:
 *   edge - Pointer to the ActorEdge structure.
 */
void free_edge(ActorEdge *edge) {
  free(edge->movie);
  free(edge);
}

/*
 * free_movie(movie)
 * Frees the memory allocated for a single MovieNode structure.
 *
 * Arguments:
 *   movie - Pointer to the MovieNode structure.
 */
void free_movie(MovieNode *movie) {
  free(movie->name);
  free(movie);
}

// Utility Functions

/*
 * create_actor_node(actor_name)
 * Creates a new ActorNode structure for the given actor name.
 *
 * Arguments:
 *   actor_name - Name of the actor.
 *
 * Returns:
 *   Pointer to the newly created ActorNode, or NULL on failure.
 */
ActorNode *create_actor_node(const char *actor_name) {
  ActorNode *actor = malloc(sizeof(ActorNode));
  if (actor == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!actor)
    return NULL;

  actor->name = strdup_safe(actor_name);
  actor->edges = NULL;
  actor->next = NULL;
  return actor;
}

/*
 * create_movie_node(movie_title)
 * Creates a new MovieNode structure for the given movie title.
 *
 * Arguments:
 *   movie_title - Title of the movie.
 *
 * Returns:
 *   Pointer to the newly created MovieNode, or NULL on failure.
 */
MovieNode *create_movie_node(const char *movie_title) {
  MovieNode *movie = malloc(sizeof(MovieNode));
  if (movie == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!movie)
    return NULL;

  movie->name = strdup_safe(movie_title);
  movie->next = NULL;
  return movie;
}

/*
 * strdup_safe(source)
 * Duplicates a string safely.
 *
 * Arguments:
 *   source - Pointer to the source string.
 *
 * Returns:
 *   Pointer to a newly allocated copy of the string, or NULL on failure.
 */
char *strdup_safe(const char *source) {
  if (!source)
    return NULL;

  size_t length = strlen(source);
  char *copy = malloc(length + 1);
  if (copy == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!copy)
    return NULL;

  memcpy(copy, source, length + 1);
  return copy;
}
