#include "parser.h"
#include "graph.h"
#include "safety.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *parse_makefile(const char *filename) {
  // Get file
  FILE *file = fopen(filename, "r");

  // Handle NULL file
  if (!file) {
    handle_error("ERROR: Failed to read file\n");
  }

  Node *root = NULL;
  Node *target = NULL;

  // Parsing the makefile
  char *line = NULL;
  size_t line_cap = 0;

  while (getline(&line, &line_cap, file) > 0) {
    parse_line(line, &root, &target);
    safe_free((void **)&line);
  }

  safe_free((void **)&line);
  safe_free((void **)&target);
  fclose(file);
  return root;
}

void parse_line(const char *line, Node **root, Node **target) {
  if (!line) {
    handle_error("ERROR: NULL line passed to parse_line()\n");
  }

  // Check if the line starts with a tab
  if (line[0] == '\t') {
    // Add the command to the target node
    const char *command = line + 1; // Skip the tab
    while (*command == ' ') {
      command++; // Skip leading spaces
    }

    // Handle NULL target
    if (*target) {
      add_command(*target, command);
    } else {
      handle_error("ERROR: Attempted to add command to NULL target\n");
    }
    return;
  }

  const char *colon = strchr(line, ':');

  // Handle bad format line
  if (!colon) {
    handle_error("ERROR: makeFile missing colon\n");
  }

  // Pull target name from line
  size_t target_len = colon - line;
  while (line[target_len - 1] == ' ') {
    target_len--;
  }
  char *target_name = safe_malloc(target_len + 1);
  strncpy(target_name, line, target_len);
  target_name[target_len] = '\0';

  // Handle empty root
  if (!*root) {
    *root = create_node("root");
  }

  *target = find_target(*root, target_name);

  // Handle target not found
  if (!*target) {
    *target = create_node(target_name);

    // Add to root
    Node *tail = (*root)->next;
    if (!tail) {
      (*root)->next = *target;
    } else {
      while (tail->next) {
        tail = tail->next;
      }

      tail->next = *target;
    }
  }

  const char *dependencies = colon + 1;
  while (*dependencies == ' ') {
    dependencies++;
  }

  char *buffer = safe_malloc(65);
  size_t buffer_index = 0;

  for (const char *ptr = dependencies;; ptr++) {
    if (*ptr == ' ' || *ptr == '\0') {
      // End of a dependency word
      if (buffer_index > 0) {
        buffer[buffer_index] = '\0';                  // Null-terminate the word
        add_dependency(*target, create_node(buffer)); // Add dependency
        buffer_index = 0;                             // Reset buffer
      }
      if (*ptr == '\0')
        break; // End of dependencies
    } else if (buffer_index < sizeof(buffer) - 1) {
      // Append character to buffer if there's space
      buffer[buffer_index++] = *ptr;
    }
  }
  safe_free((void **)&buffer);
}

Node *find_target(Node *node, const char *target_name) {
  if (!node) {
    handle_error("ERROR: NULL root passed into find_target()");
  }

  // Search for target in dependency graph
  while (node) {
    if (strcmp(node->name, target_name) == 0) {
      return node;
    }
    node = node->dependencies;
  }

  // Target not found
  return NULL;
}
