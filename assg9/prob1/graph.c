/*
 * File: graph.c
 * Author: Matthew
 * Purpose: Implements a dependency graph with functionality for creating nodes,
 *          adding commands and dependencies, freeing resources, and performing
 *          a postorder traversal.
 */

#include "graph.h"
#include "safety.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * create_node(name) -- Creates a new node in the graph.
 *
 * Arguments:
 *    name - The name of the node (must not be NULL).
 *
 * Returns:
 *    A pointer to the newly created Node.
 *
 * Assumptions:
 *    - The name is non-NULL.
 *
 * Behavior:
 *    Allocates memory for a new Node and initializes its fields.
 *    Exits with an error if memory allocation or input validation fails.
 */
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
  return node;
}

/*
 * create_command(command) -- Creates a new command for a node.
 *
 * Arguments:
 *    command - The command string (must not be NULL).
 *
 * Returns:
 *    A pointer to the newly created Command.
 *
 * Assumptions:
 *    - The command is non-NULL.
 *
 * Behavior:
 *    Allocates memory for a new Command and initializes its fields.
 *    Exits with an error if memory allocation or input validation fails.
 */
Command *create_command(const char *command) {
  if (!command) {
    handle_error("ERROR: NULL value passed into create_command\n");
  }

  Command *node = (Command *)safe_malloc(sizeof(Command));
  node->command = safe_strdup(command);
  node->next = NULL;
  return node;
}

/*
 * find_dependency(target, dependency) -- Checks if a node has a specific
 * dependency.
 *
 * Arguments:
 *    target - The node whose dependencies are to be checked (must not be NULL).
 *    dependency - The name of the dependency (must not be NULL).
 *
 * Returns:
 *    1 if the dependency is found; 0 otherwise.
 *
 * Assumptions:
 *    - Both target and dependency are non-NULL.
 */
int find_dependency(Node *target, const char *dependency) {
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

/*
 * add_dependency(target, dependency) -- Adds a dependency to a target node.
 *
 * Arguments:
 *    target - The node to which the dependency is to be added (must not be
 * NULL). dependency - The dependency node to add (must not be NULL).
 *
 * Behavior:
 *    Adds the dependency to the target's dependency list.
 */
void add_dependency(Node *target, Node *dependency) {
  if (!target || !dependency) {
    handle_error("ERROR: NULL values passed into add_dependency\n");
  }

  if (!(target->dependencies)) {
    target->dependencies = dependency;
    return;
  }

  Node *dependencies = target->dependencies;
  while (dependencies->dependencies) {
    dependencies = dependencies->dependencies;
  }

  dependencies->dependencies = dependency;
}

/*
 * add_command(node, command) -- Adds a command to a node.
 *
 * Arguments:
 *    node - The node to which the command is to be added (must not be NULL).
 *    command - The command string to add (must not be NULL).
 *
 * Behavior:
 *    Adds the command to the node's command list.
 */
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

/*
 * free_graph(root) -- Frees all memory associated with the graph.
 *
 * Arguments:
 *    root - The root of the graph to free.
 *
 * Behavior:
 *    Frees all nodes, their dependencies, and commands in the graph.
 */
void free_graph(Node *root) {
  if (!root)
    return;
  free_commands(root->commands);
  free_graph(root->dependencies);
  safe_free((void **)&(root->name));
  safe_free((void **)&root);
}

/*
 * free_commands(root) -- Frees all commands in a command list.
 *
 * Arguments:
 *    root - The root of the command list to free.
 *
 * Behavior:
 *    Frees all command nodes in the list.
 */
void free_commands(Command *root) {
  if (!root)
    return;

  free_commands(root->next);
  safe_free((void **)&(root->command));
  safe_free((void **)&root);
}

/*
 * postorder_traversal(root) -- Performs a postorder traversal of the graph.
 *
 * Arguments:
 *    root - The root node from which to start the traversal (must not be NULL).
 *
 * Behavior:
 *    Visits all nodes in postorder and processes them using process_node.
 */
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

/*
 * process_node(node) -- Processes a node by printing its name and commands.
 *
 * Arguments:
 *    node - The node to process (must not be NULL).
 *
 * Behavior:
 *    Prints the node's name followed by its commands.
 */
void process_node(Node *node) {
  if (!node) {
    handle_error("ERROR: NULL node passed into process_node");
  }

  printf("%s\n", node->name);

  Command *commands = node->commands;
  while (commands) {
    printf("  %s\n", commands->command);
    commands = commands->next;
  }
}
