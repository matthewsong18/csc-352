#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ActorNameNode {
    char *name;
    struct ActorNameNode *next;
} ActorNameNode;

typedef struct MovieEntry {
    char *title;
    ActorNameNode *actors;
    struct MovieEntry *next;
} MovieEntry;

int parse_arguments(int argc, char *argv[], int *long_output, char **filename);
char **read_lines(const char *filename, int *num_lines);
MovieEntry *parse_movie_entries(char **lines, int num_lines);
int is_blank_line(const char *line);
int is_white_space(const char *line);
void free_movie_list(MovieEntry *movie_list);

#endif
