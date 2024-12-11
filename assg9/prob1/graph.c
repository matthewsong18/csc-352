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
  node->target = safe_strdup(name);
  node->commands = NULL;
  node->dependency = NULL;
  node->dependent = NULL;
  node->prev_dep = NULL;
  node->next_dep = NULL;
  node->prev_target = NULL;
  node->next_target = NULL;
  node->last_target = NULL;
  node->visited = 0;
  return node;
}

int is_command(char *line) { return (line[0] == '\t'); }

void add_command(char *line, Node *root) {
  Command *command = parse_command(line);
  if (command == NULL)
    return;
  add_command_to_end_of_list(root, command);
}

Command *parse_command(char *line) {
  char *command = line + 1;
  while (isspace(*command)) {
    command++;
  }
  if (*command == '\0')
    return NULL;
  strip_newline(command);
  return create_command(command);
}

void strip_newline(char *line) {
  char *new_line_ptr = strchr(line, '\n');
  if (new_line_ptr != NULL)
    *new_line_ptr = '\0';
}

Command *create_command(char *command) {
  Command *command_node = safe_malloc(sizeof(Command));
  command_node->command = safe_strdup(command);
  command_node->next = NULL;

  return command_node;
}

void add_command_to_end_of_list(Node *root, Command *command) {
  Node *target = root->last_target;
  if (target->commands == NULL) {
    target->commands = command;
    return;
  }
  Command *commands = target->commands;
  while (commands->next != NULL) {
    commands = commands->next;
  }
  commands->next = command;
}

char *parse_target(char *line) {
  char *buffer = safe_malloc(65);
  sscanf(line, "%64s", buffer);
  char *colon_in_buffer = strchr(buffer, ':');
  if (colon_in_buffer != NULL)
    *colon_in_buffer = '\0';
  return buffer;
}

void add_target(char *target, Node **root) {
  if (*root == NULL) {
    *root = create_node(target);
    (*root)->last_target = *root;
    return;
  }
  Node *targets = (*root)->next_target;
  if (targets == NULL) {
    (*root)->next_target = create_node(target);
    (*root)->next_target->prev_target = *root;
    (*root)->last_target = (*root)->next_target;
    return;
  }
  while (targets->next_target != NULL) {
    targets = targets->next_target;
  }
  Node *new_target = create_node(target);
  targets->next_target = new_target;
  new_target->prev_target = targets;
  (*root)->last_target = new_target;
}

void add_dependencies(char *line, Node *root, int colon) {
  Node *dependency = parse_dependencies(&line, colon);
  if (dependency == NULL)
    return;
  add_dependency(root, dependency);
  add_dependencies(line, root, 1);
}

Node *parse_dependencies(char **line, int colon) {
  char *line_ptr = *line;
  if (!colon) {
    line_ptr = strchr(*line, ':');
    if (line_ptr == NULL) {
      fprintf(stderr, "ERROR: No colon");
      exit(1);
    }
    *line = line_ptr + 1;
  }
  char *buffer = safe_malloc(65);
  // TODO FREE BUFFER
  int jump = 0;
  int length = sscanf(*line, "%64s%n", buffer, &jump);
  if (length <= 0)
    return NULL;
  *line = *line + jump;

  Node *dependency = create_node(buffer);
  // TODO Free buffer
  return dependency;
}

void add_dependency(Node *root, Node *dependency) {
  if (has_no_dependency(root->last_target, dependency))
    return;

  add_dependency_at_end_of_list(root->last_target, dependency);
}

int has_no_dependency(Node *target, Node *dependency) {
  if (target->dependency == NULL) {
    target->dependency = dependency;
    dependency->dependent = target;
    return 1;
  }
  return 0;
}

void add_dependency_at_end_of_list(Node *target, Node *dependency) {
  Node *dependency_list = target->dependency;
  if (dependency_list == dependency) {
    // TODO
  }
  while (dependency_list->next_dep != NULL) {
    if (dependency_list == dependency) {
      // TODO
    }
    dependency_list = dependency_list->next_dep;
  }
  dependency_list->next_dep = dependency;
  dependency->prev_dep = dependency_list;
}

void update_tree(Node **root_addr) {
  Node *root = *root_addr;
  Node *search_target = root;
  search_for_all_targets(search_target, root);
}

void search_for_all_targets(Node *search_target, Node *root) {
  if (search_target == NULL) {
    return;
  }

  Node *targets = root;
  Node *found_node = search_every_target(search_target, targets);
  if (found_node != NULL) {
    replace_node(found_node, search_target);
  }

  search_for_all_targets(search_target->next_target, root);
}

Node *search_every_target(Node *search_target, Node *targets) {
  while (targets != NULL) {
    Node *found_node = find_node(search_target, targets);
    if (found_node != NULL) {
      return found_node;
    }
    targets = targets->next_target;
  }
  return NULL;
}

Node *find_node(Node *search_target, Node *root) {
  if (root == NULL)
    return NULL;
  if (root == search_target) {
    return NULL;
  }
  if (strcmp(search_target->target, root->target) == 0)
    return root;

  Node *dependencies = root->dependency;
  search_dependencies(search_target, dependencies);
  return NULL;
}

Node *search_dependencies(Node *search_target, Node *root) {
  Node *search = find_node(search_target, root);
  if (search != NULL)
    return search;
  search_dependencies(search_target, root->next_dep);
  return NULL;
}

void replace_node(Node *replaced_node, Node *new_node) {
  replace_dependent(replaced_node, new_node);
  replace_prev_dep(replaced_node, new_node);
  replace_next_dep(replaced_node, new_node);
  replace_targets(replaced_node, new_node);
}

void replace_dependent(Node *replaced_node, Node *new_node) {
  Node *dependent = replaced_node->dependent;
  if (dependent != NULL) {
    dependent->dependency = new_node;
    new_node->dependent = dependent;
  }
}

void replace_prev_dep(Node *replaced_node, Node *new_node) {
  Node *prev = replaced_node->prev_dep;
  if (prev != NULL) {
    prev->next_dep = new_node;
    new_node->prev_dep = prev;
  }
}

void replace_next_dep(Node *replaced_node, Node *new_node) {
  Node *next = replaced_node->next_dep;
  if (next != NULL) {
    next->prev_dep = new_node;
    new_node->next_dep = next;
  }
}

void replace_targets(Node *replaced_node, Node *new_node) {
  Node *prev_target = new_node->prev_target;
  Node *next_target = new_node->next_target;

  if (prev_target != NULL && next_target != NULL) {
    prev_target->next_target = next_target;
    next_target->prev_target = prev_target;
  } else if (prev_target != NULL) {
    prev_target->next_target = NULL;
  } else if (next_target != NULL) {
    next_target->prev_target = NULL;
  }
  new_node->prev_target = NULL;
  new_node->next_target = NULL;
  new_node->last_target = NULL;
}

void postorder_traversal(char *target_name, Node *root) {
  Node *search_target = create_node(target_name);
  Node *start = find_node(search_target, root);
  DFS(start);
}

void DFS(Node *root) {
  if (root == NULL)
    return;
  if (root->visited == 1)
    return;

  Node *dependencies = root->dependency;
  if (dependencies != NULL) {
    while (dependencies != NULL) {
      dependencies->visited = 1;
      DFS(dependencies->dependency);
      printf("%s\n", dependencies->target);
      Command *commands = dependencies->commands;
      while (commands != NULL) {
        printf("\t%s\n", commands->command);
        commands = commands->next;
      }
      dependencies = dependencies->next_dep;
    }
  }

  printf("%s\n", root->target);

  Command *commands = root->commands;
  while (commands != NULL) {
    printf("\t%s\n", commands->command);
    commands = commands->next;
  }

  return;
}
