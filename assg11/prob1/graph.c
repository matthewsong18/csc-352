#include "graph.h"
#include "file_parser.h"
#include <stdlib.h>
#include <string.h>

// Main Functions

Graph *init_graph() { return create_empty_graph(); }

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

void free_graph(Graph *graph) {
  if (!graph)
    return;
  free_actor_list(graph->actors);
  free_movie_node_list(graph->movies);
  free(graph);
}

ActorNode *find_actor(Graph *graph, const char *actor_name) {
  return search_actor_list(graph->actors, actor_name);
}

void link_actors(ActorNode *actor1, ActorNode *actor2,
                 const char *movie_title) {
  create_bidirectional_edge(actor1, actor2, movie_title);
}

// Helper Functions for Graph Initialization

Graph *create_empty_graph() {
  Graph *graph = malloc(sizeof(Graph));
  if (!graph)
    return NULL;

  graph->actors = NULL;
  graph->movies = NULL;
  return graph;
}

// Helper Functions for Graph Population

int populate_graph_from_movies(Graph *graph, MovieEntry *movie_list) {
  while (movie_list) {
    if (!process_single_movie(graph, movie_list))
      return 0;
    movie_list = movie_list->next;
  }
  return 1;
}

int process_single_movie(Graph *graph, MovieEntry *movie) {
  ActorNameNode *first_actor = movie->actors;
  while (first_actor) {
    if (!process_movie_actor(graph, first_actor, movie->title))
      return 0;
    first_actor = first_actor->next;
  }
  return 1;
}

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

int link_movie_actors(Graph *graph, ActorNode *actor1,
                      ActorNameNode *actor2_name_node,
                      const char *movie_title) {
  ActorNode *actor2 = get_or_create_actor(graph, actor2_name_node->name);
  if (!actor2)
    return 0;

  link_actors(actor1, actor2, movie_title);
  return 1;
}

ActorNode *get_or_create_actor(Graph *graph, const char *actor_name) {
  ActorNode *existing_actor = find_actor(graph, actor_name);
  if (existing_actor)
    return existing_actor;

  return create_and_add_actor(graph, actor_name);
}

ActorNode *create_and_add_actor(Graph *graph, const char *actor_name) {
  ActorNode *new_actor = create_actor_node(actor_name);
  if (!new_actor)
    return NULL;

  new_actor->next = graph->actors;
  graph->actors = new_actor;
  return new_actor;
}

// Helper Functions for Actor and Movie Lists

void free_actor_list(ActorNode *actor_list) {
  while (actor_list) {
    ActorNode *next_actor = actor_list->next;
    free_actor(actor_list);
    actor_list = next_actor;
  }
}

void free_movie_node_list(MovieNode *movie_list) {
  while (movie_list) {
    MovieNode *next_movie = movie_list->next;
    free_movie(movie_list);
    movie_list = next_movie;
  }
}

ActorNode *search_actor_list(ActorNode *actor_list, const char *actor_name) {
  while (actor_list) {
    if (strcmp(actor_list->name, actor_name) == 0)
      return actor_list;
    actor_list = actor_list->next;
  }
  return NULL;
}

// Helper Functions for Edges

void create_bidirectional_edge(ActorNode *actor1, ActorNode *actor2,
                               const char *movie_title) {
  create_edge(actor1, actor2, movie_title);
  create_edge(actor2, actor1, movie_title);
}

void create_edge(ActorNode *from_actor, ActorNode *to_actor,
                 const char *movie_title) {
  ActorEdge *new_edge = malloc(sizeof(ActorEdge));
  if (!new_edge)
    return;

  new_edge->actor = to_actor;
  new_edge->movie = strdup_safe(movie_title);
  new_edge->next = from_actor->edges;
  from_actor->edges = new_edge;
}

// Helper Functions for Memory Management

void free_actor(ActorNode *actor) {
  free(actor->name);
  free_edge_list(actor->edges);
  free(actor);
}

void free_edge_list(ActorEdge *edge_list) {
  while (edge_list) {
    ActorEdge *next_edge = edge_list->next;
    free_edge(edge_list);
    edge_list = next_edge;
  }
}

void free_edge(ActorEdge *edge) {
  free(edge->movie);
  free(edge);
}

void free_movie(MovieNode *movie) {
  free(movie->name);
  free(movie);
}

// Utility Functions

ActorNode *create_actor_node(const char *actor_name) {
  ActorNode *actor = malloc(sizeof(ActorNode));
  if (!actor)
    return NULL;

  actor->name = strdup_safe(actor_name);
  actor->edges = NULL;
  actor->next = NULL;
  return actor;
}

MovieNode *create_movie_node(const char *movie_title) {
  MovieNode *movie = malloc(sizeof(MovieNode));
  if (!movie)
    return NULL;

  movie->name = strdup_safe(movie_title);
  movie->next = NULL;
  return movie;
}

char *strdup_safe(const char *source) {
  if (!source)
    return NULL;

  size_t length = strlen(source);
  char *copy = malloc(length + 1);
  if (!copy)
    return NULL;

  memcpy(copy, source, length + 1);
  return copy;
}
