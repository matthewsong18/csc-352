#ifndef PARSER_H
#define PARSER_H

#include "graph.h"
#include "safety.h"

// Function prototypes
Node *parse_makefile(const char *filename);
void parse_line(const char *line, Node **root, Node **target);
Node *find_target(Node *root, const char *target_name);

#endif // PARSER_H
