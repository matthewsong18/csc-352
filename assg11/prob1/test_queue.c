#include "graph.h"
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Since we need ActorNode for testing, we might create some mock ActorNodes
// here.
static ActorNode *mock_actor(const char *name) {
  ActorNode *a = malloc(sizeof(ActorNode));
  assert(a != NULL);
  a->name = strdup(name);
  a->edges = NULL;
  a->next = NULL;
  // If BFS fields exist, you can initialize them here,
  // but it's not necessary for just queue testing.
  return a;
}

// This function tests the queue operations.
int test_queue_ops() {
  Queue *q = init_queue();
  assert(q != NULL);

  // Initially, queue should be empty
  assert(dequeue(q) == NULL);

  // Create three mock actors
  ActorNode *kevin = mock_actor("Kevin Bacon");
  ActorNode *ed = mock_actor("Ed Harris");
  ActorNode *lori = mock_actor("Lori Singer");

  // Enqueue actors
  enqueue(q, kevin);
  enqueue(q, ed);
  enqueue(q, lori);

  // Dequeue should return them in FIFO order
  ActorNode *a = dequeue(q);
  assert(a == kevin);

  a = dequeue(q);
  assert(a == ed);

  a = dequeue(q);
  assert(a == lori);

  // Now queue is empty again
  assert(dequeue(q) == NULL);

  // Cleanup
  // The queue is empty, just free it
  free_queue(q);

  // Free the mock actors
  free(kevin->name);
  free(kevin);
  free(ed->name);
  free(ed);
  free(lori->name);
  free(lori);

  return 0;
}

int main(void) {
  test_queue_ops();

  printf("All test_queue tests passed!\n");
  fflush(stdout);
  return 0;
}
