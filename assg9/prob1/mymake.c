#include "mymake.h"

int main(int argc, char **argv) {
  int errorStatus = STATUS_OKAY;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s <makefile> <target>\n", argv[0]);
    exit(1);
  }

  const char *makefile = argv[1];
  const char *target_name = argv[2];

  // Parse the makefile and build the dependency graph
  Node *root = parse_makefile(makefile);
  if (!root) {
    handle_error("Failed to parse makefile.");
  }

  // Find the specified target in the graph
  Node *target = find_target(root, target_name);
  if (!target) {
    fprintf(stderr, "Target '%s' not found in makefile.\n", target_name);
    exit(1);
  }

  // Perform postorder traversal
  postorder_traversal(target);

  // Free allocated memory
  free_graph(root->next);
  free_graph(root);

  return errorStatus;
}
