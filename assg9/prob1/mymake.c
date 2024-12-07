/*
 * File: mymake.c
 * Author: Matthew
 * Purpose: Implements a simple dependency resolver for makefiles. The program
 *          parses a makefile, builds a dependency graph, and performs a
 *          postorder traversal on the specified target.
 */

#include "mymake.h"
#include "graph.h"
#include "parser.h"

int main(int argc, char **argv) {
  Node *root = make_dependency_tree(argv[1]);
  postorder_traversal(argv[2], root);
}
