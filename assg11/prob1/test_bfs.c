#include "bfs.h"
#include "graph.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_bfs() {
  Graph *g = init_graph();
  assert(g != NULL);

  // Create three actors:
  ActorNode *kevin = create_actor_node("Kevin Bacon");
  ActorNode *ed = create_actor_node("Ed Harris");
  ActorNode *lori = create_actor_node("Lori Singer");

  // Insert them into graph
  kevin->next = g->actors;
  g->actors = kevin;
  ed->next = g->actors;
  g->actors = ed;
  lori->next = g->actors;
  g->actors = lori;

  // Link them
  link_actors(kevin, ed, "Apollo 13");
  link_actors(kevin, lori, "Footloose");

  // Initialize BFS with Kevin Bacon as the start
  int result = init_bfs(g, "Kevin Bacon");
  assert(result == 0);

  // Now create a queue and enqueue Kevin Bacon
  Queue *q = init_queue();
  assert(q != NULL);
  enqueue(q, kevin);

  // Run BFS
  result = run_bfs(g, q);
  assert(result == 0);

  // Check distances:
  // Kevin Bacon should be distance 0
  // Ed Harris and Lori Singer should be distance 1 (directly connected to Kevin
  // Bacon)
  assert(get_bacon_score(g, "Kevin Bacon") == 0);
  assert(get_bacon_score(g, "Ed Harris") == 1);
  assert(get_bacon_score(g, "Lori Singer") == 1);

  // Check a non-existing actor
  assert(get_bacon_score(g, "Tom Hanks") == -1);

  // Cleanup
  free_queue(q);
  free_graph(g);

  return 0;
}

int test_reconstruct_path() {
  Graph *g = init_graph();
  assert(g != NULL);

  // Create nodes
  ActorNode *kevin = create_actor_node("Kevin Bacon");
  ActorNode *ed = create_actor_node("Ed Harris");
  ActorNode *lori = create_actor_node("Lori Singer");
  ActorNode *sean = create_actor_node("Sean Connery");

  // Link actors into graph
  kevin->next = g->actors;
  g->actors = kevin;
  ed->next = g->actors;
  g->actors = ed;
  lori->next = g->actors;
  g->actors = lori;
  sean->next = g->actors;
  g->actors = sean;

  // Link them with movies
  link_actors(kevin, ed, "Apollo 13");
  link_actors(kevin, lori, "Footloose");
  link_actors(ed, sean, "The Rock");

  // Run BFS from Kevin Bacon
  assert(init_bfs(g, "Kevin Bacon") == 0);
  Queue *q = init_queue();
  enqueue(q, kevin);
  assert(run_bfs(g, q) == 0);

  // Check distances:
  // Kevin Bacon = 0
  // Ed Harris, Lori Singer = 1
  // Sean Connery = 2
  assert(get_bacon_score(g, "Kevin Bacon") == 0);
  assert(get_bacon_score(g, "Ed Harris") == 1);
  assert(get_bacon_score(g, "Lori Singer") == 1);
  assert(get_bacon_score(g, "Sean Connery") == 2);

  // Test reconstructing path to Sean Connery
  char **actor_chain = NULL;
  char **movie_chain = NULL;
  int chain_length = 0;

  int result = reconstruct_path(g, "Sean Connery", &actor_chain, &movie_chain,
                                &chain_length);
  assert(result == 0);
  // chain_length should be 2: Kevin->Ed, Ed->Sean
  // actor_chain should be: ["Kevin Bacon", "Ed Harris", "Sean Connery"]
  // movie_chain should be: ["Apollo 13", "The Rock"]

  assert(chain_length == 2);
  assert(strcmp(actor_chain[0], "Kevin Bacon") == 0);
  assert(strcmp(actor_chain[1], "Ed Harris") == 0);
  assert(strcmp(actor_chain[2], "Sean Connery") == 0);

  assert(strcmp(movie_chain[0], "Apollo 13") == 0);
  assert(strcmp(movie_chain[1], "The Rock") == 0);

  // Cleanup
  for (int i = 0; i <= chain_length; i++) {
    free(actor_chain[i]);
  }
  free(actor_chain);
  for (int i = 0; i < chain_length; i++) {
    free(movie_chain[i]);
  }
  free(movie_chain);

  free_queue(q);
  free_graph(g);

  printf("test_reconstruct_path passed!\n");
  return 0;
}

int main(void) {
  test_bfs();
  test_reconstruct_path();

  printf("All test_bfs tests passed!\n");
  fflush(stdout);
  return 0;
}
