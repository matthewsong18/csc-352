#include "bfs.h"
#include "graph.h"
#include "queue.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For pipe(), dup2(), and fork()

// Helper to create a mock graph
static Graph *create_mock_graph() {
  Graph *graph = init_graph();
  assert(graph != NULL);

  ActorNode *kevin = create_actor_node("Kevin Bacon");
  ActorNode *ed = create_actor_node("Ed Harris");
  ActorNode *lori = create_actor_node("Lori Singer");
  ActorNode *sean = create_actor_node("Sean Connery");

  kevin->next = graph->actors;
  graph->actors = kevin;
  ed->next = graph->actors;
  graph->actors = ed;
  lori->next = graph->actors;
  graph->actors = lori;
  sean->next = graph->actors;
  graph->actors = sean;

  link_actors(kevin, ed, "Apollo 13");
  link_actors(kevin, lori, "Footloose");
  link_actors(ed, sean, "The Rock");

  // Run BFS to set up distances and parent pointers
  assert(init_bfs(graph, "Kevin Bacon") == 0);
  Queue *queue = init_queue();
  enqueue(queue, kevin);
  assert(run_bfs(graph, queue) == 0);

  free_queue(queue);
  return graph;
}

// Helper to capture the output of a function
static char *capture_output(void (*func)(Graph *, int), Graph *graph,
                            int long_output, const char *input) {
  int pipe_fd[2];
  assert(pipe(pipe_fd) == 0);

  // Redirect stdout to the write end of the pipe
  int saved_stdout = dup(STDOUT_FILENO);
  dup2(pipe_fd[1], STDOUT_FILENO);
  close(pipe_fd[1]);

  // Simulate user input by redirecting stdin
  int pipe_in[2];
  assert(pipe(pipe_in) == 0);
  int saved_stdin = dup(STDIN_FILENO);
  dup2(pipe_in[0], STDIN_FILENO);
  close(pipe_in[0]);
  write(pipe_in[1], input, strlen(input));
  close(pipe_in[1]);

  // Run the function
  func(graph, long_output);

  // Restore stdout and stdin
  fflush(stdout);
  dup2(saved_stdout, STDOUT_FILENO);
  close(saved_stdout);

  dup2(saved_stdin, STDIN_FILENO);
  close(saved_stdin);

  // Read from the pipe
  char *output = malloc(4096);
  int bytes_read = read(pipe_fd[0], output, 4095);
  close(pipe_fd[0]);
  if (bytes_read >= 0)
    output[bytes_read] = '\0';
  return output;
}

// Test case for handle_queries
void test_handle_queries() {
  Graph *graph = create_mock_graph();

  const char *input = "Ed Harris\nSean Connery\nUnknown Actor\n";
  char *output = capture_output(handle_queries, graph, 1, input);

  // Validate output
  assert(strstr(output, "Ed Harris: Bacon number is 1") != NULL);
  assert(strstr(output, "Sean Connery: Bacon number is 2") != NULL);
  assert(strstr(output, "Unknown Actor: unreachable") != NULL);
  assert(strstr(output, "Path: Kevin Bacon -> (Apollo 13) -> Ed Harris") !=
         NULL);
  assert(strstr(output, "Ed Harris -> (The Rock) -> Sean Connery") != NULL);

  free(output);
  free_graph(graph);
  printf("test_handle_queries passed!\n");
}

int main(void) {
  test_handle_queries();
  printf("All tests passed!\n");
  return 0;
}
