#ifndef GRAPH_H
#define GRAPH_H

#include "file_parser.h"

typedef struct ActorNode ActorNode;
typedef struct ActorEdge ActorEdge;
typedef struct MovieNode MovieNode;
typedef struct Graph Graph;

struct ActorEdge {
  ActorNode *actor;
  char *movie;
  ActorEdge *next;
};

struct ActorNode {
  char *name;
  ActorEdge *edges;
  ActorNode *next;
  int distance;
  ActorNode *parent;
};

struct MovieNode {
  char *name;
  MovieNode *next;
};

struct Graph {
  ActorNode *actors;
  MovieNode *movies;
};

// Main Functions
Graph *init_graph();
Graph *build_actor_graph(MovieEntry *movie_list);
void free_graph(Graph *graph);
ActorNode *find_actor(Graph *graph, const char *actor_name);
void link_actors(ActorNode *actor1, ActorNode *actor2, const char *movie_title);

// Helper Functions for Graph Initialization
Graph *create_empty_graph();

// Helper Functions for Graph Population
int populate_graph_from_movies(Graph *graph, MovieEntry *movie_list);
int process_single_movie(Graph *graph, MovieEntry *movie);
int process_movie_actor(Graph *graph, ActorNameNode *actor_name_node,
                        const char *movie_title);
int link_movie_actors(Graph *graph, ActorNode *actor1,
                      ActorNameNode *actor2_name_node, const char *movie_title);

// Helper Functions for Actors and Movies
ActorNode *get_or_create_actor(Graph *graph, const char *actor_name);
ActorNode *create_and_add_actor(Graph *graph, const char *actor_name);
void free_actor_list(ActorNode *actor_list);
void free_movie_node_list(MovieNode *movie_list);
ActorNode *search_actor_list(ActorNode *actor_list, const char *actor_name);

// Helper Functions for Edges
void create_bidirectional_edge(ActorNode *actor1, ActorNode *actor2,
                               const char *movie_title);
void create_edge(ActorNode *from_actor, ActorNode *to_actor,
                 const char *movie_title);

// Helper Functions for Memory Management
void free_actor(ActorNode *actor);
void free_edge_list(ActorEdge *edge_list);
void free_edge(ActorEdge *edge);
void free_movie(MovieNode *movie);

// Utility Functions
ActorNode *create_actor_node(const char *actor_name);
MovieNode *create_movie_node(const char *movie_title);
char *strdup_safe(const char *source);

#endif
