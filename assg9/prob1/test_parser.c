#include "parser.h"
#include <stdio.h>
#include <string.h>

// Function prototypes for individual tests
int test_parse_line();
int test_parse_example_makefile();
int test_find_target_in_example();

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
  int total_tests = 3, passed_tests = 0;

  printf("Starting Tests...\n");
  if (test_parse_line())
    passed_tests++;
  if (test_parse_example_makefile())
    passed_tests++;
  if (test_find_target_in_example())
    passed_tests++;

  printf("Test Summary: %d/%d tests passed.\n", passed_tests, total_tests);
  return (total_tests == passed_tests)
             ? 0
             : 1; // Exit with failure if not all tests pass
}

// Test Cases Implementation

int test_parse_line() {
  printf("Testing parse line\n");

  Node *root = NULL;
  Node *target = NULL;
  char *line = "spellcheck.o : utils.h spellcheck.h spellcheck.c";
  parse_line(line, &root, &target);
  if (!root) {
    printf("ERROR: Root is NULL\n");
    safe_free((void **)&target);
    return 0;
  }

  if (!target) {
    printf("ERROR: Target is NULL\n");
    safe_free((void **)&root);
    return 0;
  }

  if (strcmp(root->next->name, "spellcheck.o") != 0) {
    printf("ERROR: root target list was not correct\n");
    printf("%s\n", root->next->name);
    safe_free((void **)&root);
    safe_free((void **)&target);
    return 0;
  }

  if (strcmp(target->name, "spellcheck.o") != 0) {
    printf("ERROR: target was not correct\n");
    printf("%s\n", target->name);
    safe_free((void **)&root);
    safe_free((void **)&target);
    return 0;
  }

  return 1;
}

int test_parse_example_makefile() {
  printf("Testing parsing example makefile...\n");

  // Parse the example makefile
  const char *example_makefile = "tests/test_example-make";
  Node *graph = parse_makefile(example_makefile);
  if (!graph) {
    printf("Error: Failed to parse example makefile.\n");
    return 0;
  }

  // Validate the structure of the graph
  Node *targets = graph->next;
  Node *target = NULL;
  while (targets) {
    if (strcmp("spellcheck.o", targets->name) == 0) {
      target = targets;
      break;
    }
    targets = targets->next;
  }

  if (!target) {
    printf("ERROR: target is NULL\n");
    return 0;
  }

  int count = 0;
  Node *depend = target->dependencies;
  while (depend) {
    count++;
    printf("%64s\n", depend->name);
    depend = depend->dependencies;
  }

  if (!target || count != 3) {
    printf("Error: spellcheck.o node has incorrect dependencies.\n");
    free_graph(graph);
    return 0;
  }

  // Check if dependencies are correct
  if (strcmp(target->dependencies->name, "utils.h") != 0) {
    printf("Error: spellcheck.o dependencies are incorrect.\n");
    free_graph(graph);
    return 0;
  }

  if (strcmp(target->dependencies->name, "spellcheck.h") != 0) {
    printf("Error: spellcheck.o dependencies are incorrect.\n");
    free_graph(graph);
    return 0;
  }
  if (strcmp(target->dependencies->name, "spellcheck.c") != 0) {
    printf("Error: spellcheck.o dependencies are incorrect.\n");
    free_graph(graph);
    return 0;
  }

  free_graph(graph);
  return 1;
}

int test_find_target_in_example() {
  printf("Testing finding targets in example makefile...\n");

  // Parse the example makefile
  const char *example_makefile = "tests/test_example-make";
  Node *graph = parse_makefile(example_makefile);
  if (!graph) {
    printf("Error: Failed to parse example makefile.\n");
    return 0;
  }

  // Validate specific targets
  Node *spellcheck = find_target(graph, "spellcheck");
  Node *testStuff = find_target(graph, "testStuff");

  if (!spellcheck || strcmp(spellcheck->name, "spellcheck") != 0) {
    printf("Error: spellcheck target not found or incorrect.\n");
    free_graph(graph);
    return 0;
  }

  if (!testStuff || strcmp(testStuff->name, "testStuff") != 0) {
    printf("Error: testStuff target not found or incorrect.\n");
    free_graph(graph);
    return 0;
  }

  free_graph(graph);
  return 1;
}
