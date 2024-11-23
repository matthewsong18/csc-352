#include "graph.h"
#include "safety.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Create a new node
Node *create_node(const char *name) {
  if (!name) {
    handle_error("ERROR: NULL value passed into create_node\n");
  }

  Node *node = (Node *)safe_malloc(sizeof(Node));
  node->name = safe_strdup(name);
  node->commands = NULL;
  node->dependencies = NULL;
  node->visited = 0;
  node->next = NULL;
  node->freed = 0;
  return node;
}

// Create a new command
Command *create_command(const char *command) {
  if (!command) {
    handle_error("ERROR: NULL value passed into create_command\n");
  }

  Command *node = (Command *)safe_malloc(sizeof(Command));
  node->command = safe_strdup(command);
  node->next = NULL;
  return node;
}

int find_dependency(Node *target, const char *dependency) {
  // Handle bad input
  if (!target || !dependency) {
    handle_error("ERROR: NULL values passed into find_dependency\n");
  }

  Node *dependencies = target->dependencies;
  while (dependencies) {
    if (strcmp(dependencies->name, dependency) == 0) {
      return 1;
    }
    dependencies = dependencies->dependencies;
  }
  return 0;
}

// Add a dependency to a node
void add_dependency(Node *target, Node *dependency) {
  // Handle bad input
  if (!target || !dependency) {
    handle_error("ERROR: NULL values passed into add_dependency\n");
  }

  // Handle empty dependencies
  if (!(target->dependencies)) {
    target->dependencies = dependency;
    return;
  }

  // Iterate to the end of the dependency linked list
  Node *dependencies = target->dependencies;
  while (dependencies->dependencies) {
    dependencies = dependencies->dependencies;
  }

  dependencies->dependencies = dependency;
}

// Add a command to a node
void add_command(Node *node, const char *command) {
  if (!node || !command) {
    handle_error("ERROR: NULL values passed into add_command\n");
  }

  if (!node->commands) {
    node->commands = create_command(command);
    return;
  }

  Command *commands = node->commands;
  while (commands->next) {
    commands = commands->next;
  }

  commands->next = create_command(command);
}

// Free the graph and its nodes
void free_graph(Node *root) {
  if (!root)
    return;

  if (root->freed)
    return;

  root->freed = 1;

  // Free dependencies as a linked list
  Node *current_dep = root->dependencies;
  while (current_dep) {
    Node *next_dep = current_dep->next; // Save next node
    free_graph(current_dep); // Recursively free the current dependency
    current_dep = next_dep;  // Move to the next dependency
  }

  // Free commands
  free_commands(root->commands);

  // Free the next node in the list
  free_graph(root->next);

  // Free the current node's name and the node itself
  safe_free((void **)&(root->name));
  safe_free((void **)&root);
}

// Free the command list
void free_commands(Command *root) {
  while (root) {
    Command *next = root->next;           // Save next command
    safe_free((void **)&(root->command)); // Free command string
    safe_free((void **)&root);            // Free command node
    root = next;                          // Move to the next command
  }
}

// Postorder traversal
void postorder_traversal(Node *root) {
  if (!root) {
    handle_error("ERROR: NULL root passed into postorder_traversal\n");
  }

  if (root->visited)
    return;

  root->visited = 1;

  Node *dependencies = root->dependencies;
  while (dependencies) {
    postorder_traversal(dependencies);
    dependencies = dependencies->dependencies;
  }
  process_node(root);
}

void process_node(Node *node) {
  if (!node) {
    handle_error("ERROR: NULL node passed into process_node");
  }

  printf("%s\n", node->name);

  Command *commands = node->commands;
  while (commands) {
    printf("  %s", commands->command);
    commands = commands->next;
  }
  return;
}
