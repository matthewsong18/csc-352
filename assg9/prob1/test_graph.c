#include "graph.h"
#include "safety.h"
#include <stdio.h>
#include <string.h>

// Function prototypes for individual tests
int test_create_node();
int test_add_dependency();
int test_add_command();
int test_postorder_traversal();
int test_free_graph();

void run_test(const char *test_name, int (*test_func)()) {
  printf("Running %s...\n", test_name);
  int result = test_func();
  if (result) {
    printf("Result: %s passed!\n\n", test_name);
  } else {
    printf("Result: %s failed.\n\n", test_name);
  }
}

int main() {
  int total_tests = 5, passed_tests = 0;

  printf("Starting Tests...\n");
  if (test_create_node())
    passed_tests++;
  if (test_add_dependency())
    passed_tests++;
  if (test_add_command())
    passed_tests++;
  if (test_postorder_traversal())
    passed_tests++;
  if (test_free_graph())
    passed_tests++;

  printf("Test Summary: %d/%d tests passed.\n", passed_tests, total_tests);
  return (total_tests == passed_tests)
             ? 0
             : 1; // Exit with failure if not all tests pass
}

// Test Cases Implementation

int test_create_node() {
  printf("Testing node creation...\n");
  Node *node = create_node("test_node");

  if (node == NULL || strcmp(node->name, "test_node") != 0) {
    printf("ERROR: Node creation failed or name mismatch.\n");
    free_graph(node);
    return 0;
  }

  if (node->commands) {
    printf("ERROR: Node commands is not NULL\n");
  }
  if (node->dependencies) {
    printf("ERROR: Node dependencies is not NULL\n");
  }
  if (node->visited != 0) {
    printf("ERROR: Node visited is not 0\n");
  }

  free_graph(node);
  return 1;
}

int test_add_dependency() {
  printf("Testing adding dependencies...\n");
  Node *parent = create_node("parent");
  Node *child = create_node("child");

  add_dependency(parent, child);

  if (parent->dependencies == NULL || parent->dependencies != child) {
    printf("Error: Dependency was not added correctly.\n");
    free_graph(parent);
    free_graph(child);
    return 0;
  }

  free_graph(parent); // Frees both parent and child
  return 1;
}

int test_add_command() {
  printf("Testing adding commands...\n");
  Node *node = create_node("test_node");

  add_command(node, "gcc -o program program.c");

  if (node->commands == NULL ||
      strcmp(node->commands->command, "gcc -o program program.c") != 0) {
    printf("Error: Command was not added correctly.\n");
    free_graph(node);
    return 0;
  }

  free_graph(node);
  return 1;
}

int test_postorder_traversal() {
  printf("Testing postorder traversal...\n");

  // Build a small dependency graph
  Node *root = create_node("root");
  Node *child1 = create_node("child1");
  Node *child2 = create_node("child2");
  add_dependency(root, child1);
  add_dependency(root, child2);

  // Add commands to nodes
  add_command(child1, "echo Building child1");
  add_command(child2, "echo Building child2");
  add_command(root, "echo Building root");

  printf("Expected output:\n");
  printf("child2\n");
  printf("\techo Building child2\n");
  printf("child1\n");
  printf("\techo Building child1\n");
  printf("root\n");
  printf("\techo Building root\n");

  printf("Actual output:\n");
  postorder_traversal(root);
  free_graph(root);

  check_for_leaks();

  return 1;
}

int test_free_graph() {
  printf("Testing freeing the graph...\n");
  Node *root = create_node("root");
  Node *child = create_node("child");
  add_dependency(root, child);

  free_graph(root);

  // Use Valgrind or manual inspection to ensure there are no memory leaks
  printf("Check Valgrind output for memory leaks.\n");
  return 1; // Assume pass unless Valgrind detects an issue
}
