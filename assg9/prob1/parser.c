/*
 * File: parser.c
 * Author: Matthew
 * Purpose: Provides functionality to parse a makefile, build a dependency
 * graph, and locate targets within the graph.
 */

#include "parser.h"

Node *make_dependency_tree(char *make_file_path) {
  Node *root = NULL;
  parse_lines(fopen(make_file_path, "r"), &root);
  return root;
}

void parse_lines(FILE *make_file, Node **root) {
  char *line = NULL;
  size_t line_cap;
  if (getline(&line, &line_cap, make_file) <= 0)
    return;
  parse_line(line, root);
  free(line);
  line = NULL;
  parse_lines(make_file, root);
}

void parse_line(char *line, Node **root) {
  if (is_command(line)) {
    add_command(line, *root);
    return;
  }

  char *target = parse_target(line);
  add_target(target, root);
  add_dependencies(line, *root, 0);
  update_tree(root);
}
