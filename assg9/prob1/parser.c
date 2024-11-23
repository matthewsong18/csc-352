/*
 * File: parser.c
 * Author: Matthew
 * Purpose: Provides functionality to parse a makefile, build a dependency
 * graph, and locate targets within the graph.
 */

#include "parser.h"
#include "graph.h"
#include "safety.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * parse_makefile(filename) -- Parses a makefile and builds a dependency graph.
 *
 * Arguments:
 *    filename - The path to the makefile (must not be NULL).
 *
 * Returns:
 *    A pointer to the root node of the dependency graph.
 *
 * Assumptions:
 *    - The makefile exists and is readable.
 *
 * Behavior:
 *    - Reads the makefile line by line and constructs the dependency graph.
 *    - Calls parse_line() to process each line.
 *    - Frees dynamically allocated memory for lines.
 *    - Exits with an error if the file cannot be opened or read.
 */
Node *parse_makefile(const char *filename) {
  FILE *file = fopen(filename, "r");

  if (!file) {
    handle_error("ERROR: Failed to read file\n");
  }

  Node *root = NULL;
  Node *target = NULL;

  char *line = NULL;
  size_t line_cap = 0;

  while (getline(&line, &line_cap, file) > 0) {
    parse_line(line, &root, &target);
    safe_free((void **)&line);
  }

  safe_free((void **)&line);
  fclose(file);
  return root;
}

/*
 * parse_line(line, root, target) -- Parses a single line of the makefile.
 *
 * Arguments:
 *    line - The line to parse (must not be NULL).
 *    root - Pointer to the root of the dependency graph (updated if NULL).
 *    target - Pointer to the current target node (updated based on the line).
 *
 * Behavior:
 *    - If the line starts with a tab, it adds a command to the current target.
 *    - If the line contains a colon, it defines a target and its dependencies.
 *    - Dynamically allocates memory for target names and buffers.
 *    - Frees temporary buffers after use.
 */
void parse_line(const char *line, Node **root, Node **target) {
  if (!line) {
    handle_error("ERROR: NULL line passed to parse_line()\n");
  }

  if (line[0] == '\t') {
    const char *command = line + 1;
    while (*command == ' ') {
      command++;
    }

    if (*target) {
      add_command(*target, command);
    } else {
      handle_error("ERROR: Attempted to add command to NULL target\n");
    }
    return;
  }

  const char *colon = strchr(line, ':');
  if (!colon) {
    handle_error("ERROR: makeFile missing colon\n");
  }

  size_t target_len = colon - line;
  while (line[target_len - 1] == ' ') {
    target_len--;
  }
  char *target_name = safe_malloc(target_len + 1);
  strncpy(target_name, line, target_len);
  target_name[target_len] = '\0';

  if (!*root) {
    *root = create_node("root");
  }

  *target = create_node(target_name);
  safe_free((void **)&target_name);

  Node *tail = (*root)->next;
  if (!tail) {
    (*root)->next = *target;
  } else {
    while (tail->next) {
      tail = tail->next;
    }
    tail->next = *target;
  }

  const char *dependencies = colon + 1;
  while (*dependencies == ' ') {
    dependencies++;
  }

  char *buffer = safe_malloc(65);
  size_t buffer_index = 0;
  const char *ptr = dependencies;

  while (*ptr) {
    if (*ptr == ' ') {
      if (buffer_index > 0) {
        buffer[buffer_index] = '\0';
        Node *targets = find_target(*root, buffer);
        if (targets) {
          add_dependency(*target, targets);
        } else {
          add_dependency(*target, create_node(buffer));
        }
        buffer_index = 0;
      }
    } else if (buffer_index < 64) {
      buffer[buffer_index++] = *ptr;
    }
    ptr++;
  }

  if (buffer_index > 0) {
    buffer[buffer_index] = '\0';
    Node *targets = find_target(*root, buffer);
    if (targets) {
      add_dependency(*target, targets);
    } else {
      add_dependency(*target, create_node(buffer));
    }
  }

  safe_free((void **)&buffer);
}

/*
 * find_target(root, target_name) -- Finds a target node by name in the graph.
 *
 * Arguments:
 *    root - The root node of the dependency graph (must not be NULL).
 *    target_name - The name of the target to find (must not be NULL).
 *
 * Returns:
 *    A pointer to the target node if found, or NULL if not found.
 *
 * Assumptions:
 *    - The graph is properly constructed.
 *
 * Behavior:
 *    - Traverses the linked list of nodes to locate the target.
 */
Node *find_target(Node *root, const char *target_name) {
  if (!root) {
    handle_error("ERROR: NULL root passed into find_target()");
  }

  Node *targets = root->next;
  while (targets) {
    if (strcmp(targets->name, target_name) == 0) {
      return targets;
    }
    targets = targets->next;
  }

  return NULL;
}
