#ifndef GRAPH_H
#define GRAPH_H

#include "safety.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure for the dependency graph
typedef struct Node {
  char *target;             // Name of the target or dependency
  struct Command *commands; // Commands associated with the target
  struct Node *dependency;  // Pointers to own first dependency
  struct Node *dependent;   // Pointer to dependent
  struct Node *prev_dep;    // Pointer to dependent's previous dependency
  struct Node *next_dep;    // Pointer to dependent's next dependency
  struct Node *prev_target; // The previous target in the tree
  struct Node *next_target; // The next target in the tree
  struct Node *last_target; // Pointer to last added target
  int visited;              // For tracking during traversal
} Node;

// Command node for the command linked list
typedef struct Command {
  char *command;        // A command line
  struct Command *next; // The next command in the linked list
} Command;

// Function prototypes
Node *create_node(const char *name);
void add_command(char *line, Node *root);
Command *parse_command(char *line);
void strip_newline(char *line);
Command *create_command(char *command);
void add_command_to_end_of_list(Node *root, Command *command);
char *parse_target(char *line);
void add_target(char *target, Node **root);
void add_dependencies(char *line, Node *root, int colon);
Node *parse_dependencies(char **line, int colon);
void add_dependency(Node *target, Node *dependency);
int has_no_dependency(Node *target, Node *dependency);
void add_dependency_at_end_of_list(Node *target, Node *dependency);
void update_tree(Node **root_addr);
void search_for_all_targets(Node *search_target, Node *root);
Node *search_every_target(Node *search_target, Node *targets);
Node *find_node(Node *search_target, Node *root);
void replace_node(Node *replaced_node, Node *new_node);
void replace_dependent(Node *replaced_node, Node *new_node);
void replace_prev_dep(Node *replaced_node, Node *new_node);
void replace_next_dep(Node *replaced_node, Node *new_node);
void replace_targets(Node *replaced_node, Node *new_node);
void postorder_traversal(char *target_name, Node *root);
void DFS(Node *root); 

#endif // GRAPH_H
