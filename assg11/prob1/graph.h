#ifndef GRAPH_H
#define GRAPH_H

typedef struct ActorNode ActorNode;
typedef struct MovieNode MovieNode;
typedef struct Graph Graph;

Graph *init_graph();
ActorNode *create_actor_node(const char *name);
MovieNode *create_movie_node(const char *name);
void link_actors(ActorNode *actor1, ActorNode *actor2, const char *movie);
void free_graph(Graph *graph);

#endif
