#include "file_parser.h" // For parse_movie_entries, MovieEntry, ActorNameNode
#include "graph.h"       // For Graph, ActorNode, init_graph, link_actors, etc.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper function to find an actor in the graph (re-implemented here for
// testing)
static ActorNode *find_actor_in_graph(Graph *g, const char *name) {
  ActorNode *cur = g->actors;
  while (cur) {
    if (strcmp(cur->name, name) == 0) {
      return cur;
    }
    cur = cur->next;
  }
  return NULL;
}

// Helper function to check if two actors are connected by a given movie
static int actors_connected_by_movie(ActorNode *actor, const char *other_name,
                                     const char *movie) {
  ActorEdge *edge = actor->edges;
  while (edge) {
    if (strcmp(edge->actor->name, other_name) == 0 &&
        strcmp(edge->movie, movie) == 0) {
      return 1; // Found a connection
    }
    edge = edge->next;
  }
  return 0;
}

int test_build_actor_graph() {
  // Create sample input lines for two movies:
  // Movie: Apollo 13
  // Tom Hanks
  // Kevin Bacon
  // Ed Harris
  //
  // Movie: Footloose
  // Kevin Bacon
  // Lori Singer

  char *lines[] = {"Movie: Apollo 13", "Tom Hanks",        "Kevin Bacon",
                   "Ed Harris",        "Movie: Footloose", "Kevin Bacon",
                   "Lori Singer"};
  int num_lines = 7;

  // Parse the lines into MovieEntry structures
  MovieEntry *movies = parse_movie_entries(lines, num_lines);
  assert(movies != NULL);

  // Build the graph from the movie entries
  Graph *g = build_actor_graph(movies);
  assert(g != NULL);

  // Check that actors from Apollo 13 are connected correctly:
  // Apollo 13 actors: Tom Hanks, Kevin Bacon, Ed Harris
  // All should be interconnected by "Apollo 13"
  ActorNode *tom = find_actor_in_graph(g, "Tom Hanks");
  ActorNode *kevin = find_actor_in_graph(g, "Kevin Bacon");
  ActorNode *ed = find_actor_in_graph(g, "Ed Harris");
  ActorNode *lori = find_actor_in_graph(g, "Lori Singer");

  assert(tom != NULL);
  assert(kevin != NULL);
  assert(ed != NULL);
  assert(lori != NULL);

  // Check Apollo 13 connections
  assert(actors_connected_by_movie(tom, "Kevin Bacon", "Apollo 13"));
  assert(actors_connected_by_movie(tom, "Ed Harris", "Apollo 13"));
  assert(actors_connected_by_movie(kevin, "Tom Hanks", "Apollo 13"));
  assert(actors_connected_by_movie(kevin, "Ed Harris", "Apollo 13"));
  assert(actors_connected_by_movie(ed, "Tom Hanks", "Apollo 13"));
  assert(actors_connected_by_movie(ed, "Kevin Bacon", "Apollo 13"));

  // Check Footloose connections:
  // Kevin Bacon and Lori Singer should be connected by "Footloose"
  assert(actors_connected_by_movie(kevin, "Lori Singer", "Footloose"));
  assert(actors_connected_by_movie(lori, "Kevin Bacon", "Footloose"));

  // Cleanup
  // Free all movie data
  {
    MovieEntry *m = movies;
    while (m) {
      MovieEntry *next_m = m->next;
      free(m->title);
      ActorNameNode *a = m->actors;
      while (a) {
        ActorNameNode *next_a = a->next;
        free(a->name);
        free(a);
        a = next_a;
      }
      free(m);
      m = next_m;
    }
  }

  // Free the graph
  free_graph(g);

  return 0;
}

int main(void) {
  test_build_actor_graph();

  printf("All test_graph tests passed!\n");
  fflush(stdout);
  return 0;
}
