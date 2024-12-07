#ifndef PARSER_H
#define PARSER_H

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
Node *make_dependency_tree(char *make_file_path);
void parse_lines(FILE *make_file, Node **root);
void parse_line(char *line, Node **root);
int is_command(char *line);

#endif // PARSER_H
