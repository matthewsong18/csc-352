#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "graph.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_blank_line(const char *line);
int is_white_space(const char *line);
char *get_movie_title(const char *line);
char *get_actor_name(const char *line);
void parse_input_file(const char *filename, Graph *graph);

#endif
