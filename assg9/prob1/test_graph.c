#include "graph.h"
#include "parser.h"
#include "safety.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Function prototypes for individual tests
int test_create_node();
int test_is_command_on_valid_input();
int test_create_command();
int test_strip_newline();
int test_parse_command();
int test_add_command_to_end_of_list();
int test_add_command();
int test_empty_command();
int test_parse_target();
int test_empty_target();
int test_add_target();
int test_add_multiple_targets();
int test_has_no_dependency();
int test_add_dependency_at_end_of_list();
int test_add_dependency();
int test_add_multiple_dependencies();
int test_parse_dependencies();
int test_add_dependencies();
int test_find_node();
int test_replace_node();
int test_update_tree();
int test_multiple_updates();
int test_postorder_traversal();

void run_test(const char *test_name, int (*test_func)()) {
  printf("Running %s...\n", test_name);
  test_func();
  printf("Result: %s passed!\n\n", test_name);
}

int main() {

  printf("\033[33m\nStarting Tests...\033[0m\n\n");

  run_test("test_create_node", test_create_node);
  run_test("test_is_command_on_valid_input", test_is_command_on_valid_input);
  run_test("test_create_command", test_create_command);
  run_test("test_strip_newline", test_strip_newline);
  run_test("test_parse_command", test_parse_command);
  run_test("test_add_command_to_end_of_list", test_add_command_to_end_of_list);
  run_test("test_add_command", test_add_command);
  run_test("test_empty_command", test_empty_command);
  run_test("test_parse_target", test_parse_target);
  run_test("test_empty_target", test_empty_target);
  run_test("test_add_target", test_add_target);
  run_test("test_add_multiple_targets", test_add_multiple_targets);
  run_test("test_has_no_dependency", test_has_no_dependency);
  run_test("test_add_dependency_at_end_of_list",
           test_add_dependency_at_end_of_list);
  run_test("test_add_dependency", test_add_dependency);
  run_test("test_add_multiple_dependencies", test_add_multiple_dependencies);
  run_test("test_parse_dependencies", test_parse_dependencies);
  run_test("test_add_dependencies", test_add_dependencies);
  run_test("test_find_node", test_find_node);
  run_test("test_replace_node", test_replace_node);
  run_test("test_update_tree", test_update_tree);
  run_test("test_multiple_updates", test_multiple_updates);
  run_test("test_postorder_traversal", test_postorder_traversal);

  printf("\033[32mAll Tests Passed\033[0m\n\n");

  return 0;
}

// Test Cases Implementation

int test_create_node() {
  // It should make a Node with a target and null values
  Node *node = create_node("target5");

  assert(node != NULL);
  assert(strcmp(node->target, "target5") == 0);
  assert(node->dependency == NULL);
  assert(node->next_dep == NULL);
  assert(node->commands == NULL);
  assert(node->prev_target == NULL);
  assert(node->next_target == NULL);
  assert(node->last_target == NULL);
  assert(node->visited == 0);

  return 1;
}

int test_is_command_on_valid_input() {
  char *test_string = "\tgcc *.o -o spellcheck";

  assert(is_command(test_string) == 1);

  return 1;
}

int test_create_command() {
  char *command_string = "gcc *.o -o spellcheck";
  Command *command = create_command(command_string);

  assert(strcmp(command->command, command_string) == 0);
  assert(command->next == NULL);

  return 1;
}

int test_strip_newline() {
  char *test_string = "gcc *.o -o spellcheck\n";
  char *input_string = safe_strdup(test_string);
  char *expected_command = "gcc *.o -o spellcheck";

  strip_newline(input_string);

  assert(strcmp(expected_command, input_string) == 0);

  return 1;
}

int test_parse_command() {
  char *command_string = "\tgcc *.o -o spellcheck";
  char *input_string = safe_strdup(command_string);
  char *expected_command = "gcc *.o -o spellcheck";

  Command *command = parse_command(input_string);

  assert(strcmp(expected_command, command->command) == 0);

  return 1;
}

int test_add_command_to_end_of_list() {
  char *command_string = "gcc *.o -o spellcheck";
  char *command_input = safe_strdup(command_string);
  Node *root = create_node("t1");
  root->last_target = root;

  Command *command = create_command(command_input);
  add_command_to_end_of_list(root, command);

  char *expected_command = "gcc *.o -o spellcheck";
  assert(strcmp(expected_command, root->commands->command) == 0);

  return 1;
}

int test_add_command() {
  char *command_string = "\tgcc *.o -o spellcheck";
  char *command_input = safe_strdup(command_string);
  Node *root = create_node("t1");
  root->last_target = root;

  add_command(command_input, root);

  char *expected_command = "gcc *.o -o spellcheck";
  assert(strcmp(expected_command, root->commands->command) == 0);

  return 1;
}

int test_empty_command() {
  char *command_string = "\t";
  char *command_input = safe_strdup(command_string);
  Node *root = create_node("t1");

  add_command(command_input, root);

  assert(root->commands == NULL);

  return 1;
}

int test_parse_target() {
  char *target_line = "spellcheck.o : utils.h spellcheck.h spellcheck.c";
  char *input_line = safe_strdup(target_line);

  char *actual_target = parse_target(input_line);

  char *expected_target = "spellcheck.o";
  assert(strcmp(expected_target, actual_target) == 0);

  return 1;
}

int test_empty_target() {
  char *target_line = "";
  char *input_line = safe_strdup(target_line);

  char *actual_target = parse_target(input_line);

  char *expected_target = "";
  assert(strcmp(expected_target, actual_target) == 0);

  return 1;
}

int test_add_target() {
  char *target_line = "spellcheck.o";
  char *input_line = safe_strdup(target_line);
  Node *root = NULL;

  char *expected_target = "spellcheck.o";
  add_target(input_line, &root);

  assert(strcmp(expected_target, root->target) == 0);
  assert(root->last_target == root);

  Node *root2 = NULL;
  add_target("spellcheck.o", &root2);
  assert(root2 != NULL);
  assert(strcmp("spellcheck.o", root2->target) == 0);

  return 1;
}

int test_add_multiple_targets() {
  char *target_line = "spellcheck.o";
  char *input_line = safe_strdup(target_line);
  Node *root = NULL;
  add_target(input_line, &root);

  char *target_line2 = "bob";
  char *input_line2 = safe_strdup(target_line2);
  add_target(input_line2, &root);

  char *target_line3 = "uchiha";
  char *input_line3 = safe_strdup(target_line3);
  add_target(input_line3, &root);

  char *expected_target1 = "spellcheck.o";
  char *expected_target2 = "bob";
  char *expected_target3 = "uchiha";

  assert(strcmp(expected_target1, root->target) == 0);
  assert(root->last_target != root);

  Node *target_2 = root->next_target;
  assert(target_2 != NULL);
  assert(target_2->target != NULL);
  assert(strcmp(expected_target2, target_2->target) == 0);
  assert(target_2->prev_target == root);

  Node *target_3 = target_2->next_target;
  assert(target_3 != NULL);
  assert(target_3->target != NULL);
  assert(strcmp(expected_target3, target_3->target) == 0);
  assert(target_3->prev_target == target_2);

  return 1;
}

int test_has_no_dependency() {
  Node *target = create_node("t1");
  Node *dependency = create_node("d1");

  int result = has_no_dependency(target, dependency);

  assert(result == 1);
  assert(target->dependency == dependency);

  int result2 = has_no_dependency(target, dependency);

  assert(result2 == 0);

  return 1;
}

int test_add_dependency_at_end_of_list() {
  Node *target = create_node("t1");
  Node *dependency = create_node("d1");
  has_no_dependency(target, dependency);
  Node *new_dependency = create_node("d1");

  add_dependency_at_end_of_list(target, new_dependency);

  Node *target_dependency = target->dependency;
  assert(target_dependency->next_dep == new_dependency);

  // TODO Test duplicate
  // add_dependency_at_end_of_list(target, dependency);

  return 1;
}

int test_add_dependency() {
  // It should add a Node to the target Node's dependencies
  Node *root = NULL;
  add_target("target", &root);
  Node *dependency_node = create_node("dependency");

  add_dependency(root, dependency_node);
  Node *target = root->last_target;
  assert(target->dependency == dependency_node);

  return 1;
}

int test_add_multiple_dependencies() {
  // It should add dependencies to the end of the list
  Node *root = NULL;
  add_target("target", &root);
  Node *dependency_1 = create_node("d1");
  Node *dependency_2 = create_node("d2");
  Node *dependency_3 = create_node("d3");
  Node *target = root->last_target;
  add_dependency(target, dependency_1);
  add_dependency(target, dependency_2);
  add_dependency(target, dependency_3);

  Node *dependency_list = target->dependency;
  assert(dependency_list = dependency_1);
  dependency_list = dependency_list->next_dep;
  assert(dependency_list = dependency_2);
  dependency_list = dependency_list->next_dep;
  assert(dependency_list = dependency_3);
  dependency_list = dependency_list->next_dep;
  assert(dependency_list == NULL);

  return 1;
}

int test_parse_dependencies() {
  char *string = "  :  word1 word2 word3\n";
  char *string_input = safe_strdup(string);

  Node *word1 = parse_dependencies(&string_input, 0);
  char *expected1 = "word1";
  assert(strcmp(expected1, word1->target) == 0);

  Node *word2 = parse_dependencies(&string_input, 1);
  char *expected2 = "word2";
  assert(strcmp(expected2, word2->target) == 0);

  return 1;
}

int test_add_dependencies() {
  Node *root = NULL;
  add_target("target", &root);
  char *string = "  :  word1 word2 word3\n";
  char *string_input = safe_strdup(string);

  add_dependencies(string_input, root, 0);
  assert(root->dependency != NULL);
  Node *dependencies = root->dependency;
  assert(strcmp("word1", dependencies->target) == 0);
  assert(dependencies->next_dep != NULL);
  dependencies = dependencies->next_dep;
  assert(strcmp("word2", dependencies->target) == 0);
  assert(dependencies->next_dep != NULL);
  dependencies = dependencies->next_dep;
  assert(strcmp("word3", dependencies->target) == 0);

  Node *root2 = NULL;
  add_target("spellcheck.o", &root2);
  char *d1 = ": utils.h spellcheck.h spellcheck.c";
  char *input1 = safe_strdup(d1);
  add_dependencies(input1, root2, 0);

  Node *dep = root2->dependency;
  assert(strcmp("utils.h", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("spellcheck.h", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("spellcheck.c", dep->target) == 0);
  dep = dep->next_dep;

  return 1;
}

int test_find_node() {
  Node *root = NULL;
  add_target("t1", &root);
  add_target("t1", &root);
  Node *search = find_node(root, root);
  assert(search == NULL);

  Node *search2 = find_node(root, root->next_target);
  assert(search2 == root->next_target);

  add_target("t2", &root);
  add_target("t3", &root);
  char *dependency_string = ": t2";
  char *input = safe_strdup(dependency_string);
  add_dependencies(input, root, 0);
  Node *t2 = root->next_target->next_target;
  Node *dep_t2 = find_node(t2, t2->next_target);
  assert(t2->next_target->dependency == dep_t2);

  t2->dependency = create_node("t5");
  t2->dependency->dependency = create_node("t4");
  t2->dependency->dependency->next_dep = create_node("t3");
  Node *dep_t3 = t2->dependency->dependency->next_dep;
  Node *t3 = t2->next_target;

  Node *search3 = find_node(t3, t2);
  assert(search3 == dep_t3);

  return 1;
}

int test_replace_node() {
  Node *root = NULL;
  add_target("t1", &root);
  add_target("t2", &root);
  add_target("t3", &root);
  Node *t1 = root;
  Node *t2 = root->next_target;
  Node *t3 = root->last_target;
  t1->dependency = create_node("d1");
  t1->dependency->dependent = t1;

  replace_node(t1->dependency, t1->next_target);
  assert(root->next_target == t3);
  assert(t3->prev_target == t1);
  assert(t2->next_target == NULL);
  assert(t2->prev_target == NULL);
  assert(t1->dependency == t2);

  return 1;
}

int test_update_tree() {
  Node *root = NULL;
  add_target("target", &root);
  char *dependencies_string = ": word1 word2 word3";
  char *input = safe_strdup(dependencies_string);
  add_dependencies(input, root, 0);

  add_target("word3", &root);
  char *dependencies_string2 = ": word4";
  char *input2 = safe_strdup(dependencies_string2);
  add_dependencies(input2, root, 0);

  assert(root->last_target != root);
  assert(strcmp("word3", root->last_target->target) == 0);
  update_tree(&root);

  assert(root->last_target == root);
  assert(root->next_target == NULL);
  assert(root->dependency != NULL);
  Node *dependencies = root->dependency;
  assert(strcmp("word1", dependencies->target) == 0);
  dependencies = dependencies->next_dep;
  assert(strcmp("word2", dependencies->target) == 0);
  dependencies = dependencies->next_dep;
  assert(strcmp("word3", dependencies->target) == 0);
  assert(dependencies->dependency != NULL);
  assert(strcmp("word4", dependencies->dependency->target) == 0);

  Node *root2 = NULL;
  add_target("target1", &root2);
  update_tree(&root2);
  add_target("target2", &root2);
  char *target2_dep = ": target1";
  char *target2_input = safe_strdup(target2_dep);
  add_dependencies(target2_input, root2, 0);
  update_tree(&root2);
  assert(root2->next_target == NULL);
  assert(strcmp("target2", root2->target) == 0);

  return 1;
}

int test_multiple_updates() {
  Node *root = NULL;
  add_target("spellcheck.o", &root);
  char *d1 = ": utils.h spellcheck.h spellcheck.c";
  char *input1 = safe_strdup(d1);
  add_dependencies(input1, root, 0);
  update_tree(&root);

  add_target("hash.o", &root);
  char *d2 = ": hash.c utils.h hash.h";
  char *input2 = safe_strdup(d2);
  add_dependencies(input2, root, 0);
  update_tree(&root);

  add_target("spellcheck", &root);
  char *d3 = ": hash.o spellcheck.o";
  char *input3 = safe_strdup(d3);
  add_dependencies(input3, root, 0);
  update_tree(&root);

  assert(strcmp("spellcheck", root->target) == 0);
  Node *dep = root->dependency;
  assert(strcmp("hash.o", dep->target) == 0);
  dep = dep->next_dep;
  assert(strcmp("spellcheck.o", dep->target) == 0);
  dep = dep->next_dep;

  return 1;
}

int test_postorder_traversal() {
  Node *root = NULL;
  add_target("target", &root);
  char *dependencies_string = ": word1 word2 word3\n";
  char *input = safe_strdup(dependencies_string);
  add_dependencies(input, root, 0);
  char *command_string = "\ttarget command test\n";
  char *command_input = safe_strdup(command_string);
  add_command(command_input, root);
  add_target("word3", &root);
  char *dependencies_string2 = ": word4";
  char *input2 = safe_strdup(dependencies_string2);
  add_dependencies(input2, root, 0);
  char *command_string2 = "\tword 3 commands woooooooo\n";
  char *command_input2 = safe_strdup(command_string2);
  add_command(command_input2, root);
  update_tree(&root);

  postorder_traversal("target", root);

  return 1;
}
