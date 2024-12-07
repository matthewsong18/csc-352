#include "graph.h"
#include "parser.h"
#include "safety.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Function prototypes for individual tests
int test_parse_line();
int test_parse_lines();
int test_make_dependency_tree();

void run_test(const char *test_name, int (*test_func)()) {
  printf("Running %s...\n", test_name);
  test_func();
  printf("Result: %s passed!\n\n", test_name);
}

int main() {
  printf("\033[33m\nStarting Tests...\033[0m\n\n");

  run_test("test_parse_line", test_parse_line);
  run_test("test_parse_lines", test_parse_lines);
  run_test("make_dependency_tree", test_make_dependency_tree);

  printf("\033[32mAll Tests Passed\033[0m\n\n");

  return 0;
}

int test_parse_line() {
  char *target_line = "target5 : dep1 dep2 dep3";
  char *input = safe_strdup(target_line);
  Node *root = NULL;

  parse_line(input, &root);
  assert(root != NULL);
  assert(root->target != NULL);
  assert(strcmp("target5", root->target) == 0);
  Node *dep = root->dependency;
  assert(strcmp("dep1", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("dep2", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("dep3", dep->target) == 0);

  char *t2 = "testStuff:";
  char *input2 = safe_strdup(t2);
  Node *root2 = NULL;

  parse_line(input2, &root2);
  assert(root2 != NULL);
  assert(root2->target != NULL);
  assert(strcmp("testStuff", root2->target) == 0);
  Node *dep2 = root2->dependency;
  assert(dep2 == NULL);
  return 1;
}

int test_parse_lines() {
  FILE *file = fopen("tests/test_example-make", "r");
  Node *root = NULL;
  parse_lines(file, &root);

  assert(root != NULL);
  assert(root->target != NULL);
  assert(strcmp("spellcheck", root->target) == 0);
  Node *dep = root->dependency;
  assert(strcmp("hash.o", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("spellcheck.o", dep->target) == 0);

  return 1;
}

int test_make_dependency_tree() {
  char *path = "tests/test_example-make";

  Node *root = make_dependency_tree(path);
  assert(root != NULL);
  assert(root->target != NULL);
  assert(strcmp("spellcheck", root->target) == 0);
  Node *dep = root->dependency;
  assert(strcmp("hash.o", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("spellcheck.o", dep->target) == 0);

  return 1;
}
