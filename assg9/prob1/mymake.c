/*
 * File: mymake.c
 * Author: Matthew
 * Purpose: Implements a simple dependency resolver for makefiles. The program
 *          parses a makefile, builds a dependency graph, and performs a
 *          postorder traversal on the specified target.
 */

#include "mymake.h"
#include "safety.h"

/*
 * main(argc, argv) -- Entry point for the program.
 *
 * Arguments:
 *    argc - The number of command-line arguments.
 *    argv - An array of command-line arguments, where:
 *           argv[0] is the program name,
 *           argv[1] is the path to the makefile,
 *           argv[2] is the target to resolve.
 *
 * Returns:
 *    STATUS_OKAY (0) on success.
 *    Exits with status 1 on error.
 *
 * Assumptions:
 *    The program expects exactly two arguments after the program name:
 *    a makefile path and a target name.
 *
 * Behavior:
 *    1. Validates the command-line arguments.
 *    2. Parses the makefile and builds a dependency graph.
 *    3. Locates the specified target within the graph.
 *    4. Performs a postorder traversal starting from the target.
 *    5. Frees all allocated memory before exiting.
 */
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
  free_graph(root);

  return errorStatus;
}
