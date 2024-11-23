#ifndef GRAPH_H
#define GRAPH_H

#include "safety.h"

// Node structure for the dependency graph
typedef struct Node {
  char *name;                // Name of the target or dependency
  struct Command *commands;  // Commands associated with the target
  struct Node *dependencies; // Linked list of pointers to dependency nodes
  int visited;               // For tracking during traversal
  struct Node *next;         // The next target in the graph
  int freed;
} Node;

// Command node for the command linked list
typedef struct Command {
  char *command;        // A command line
  struct Command *next; // The next command in the linked list
} Command;

// Function prototypes
Node *create_node(const char *name);
Command *create_command(const char *command);
void add_dependency(Node *target, Node *dependency);
void add_command(Node *node, const char *command);
void free_graph(Node *root);
void free_commands(Command *root);
void postorder_traversal(Node *root);
void process_node(Node *node);

#endif // GRAPH_H
