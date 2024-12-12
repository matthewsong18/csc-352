#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data Structures
typedef struct ActorNameNode {
    char *name;
    struct ActorNameNode *next;
} ActorNameNode;

typedef struct MovieEntry {
    char *title;
    ActorNameNode *actors;
    struct MovieEntry *next;
} MovieEntry;

// Main Function Prototypes
int parse_arguments(int argc, char *argv[], int *long_output, char **filename);
char **read_lines(const char *filename, int *num_lines);
MovieEntry *parse_movie_entries(char **lines, int num_lines);
void free_movie_entry_list(MovieEntry *movie_list);

// Argument Parsing Function Prototypes
void initialize_arguments(int *long_output, char **filename);
int process_command_line_arguments(int argc, char *argv[], int *long_output, char **filename);
int is_long_output_flag(const char *arg);
int is_unknown_flag(const char *arg);

// File Reading Function Prototypes
FILE *open_file(const char *filename);
void close_file(FILE *file_pointer);
char **process_file_lines(FILE *file_pointer, int *num_lines);
int read_and_store_lines(FILE *file_pointer, char ***lines, int *num_lines, int *capacity);
int add_line(char ***lines, int *num_lines, int *capacity, const char *line);
int expand_line_array(char ***lines, int *capacity);
void free_lines(char **lines, int num_lines);

// Line Processing Function Prototypes
int is_non_blank_line(const char *line);
int is_white_space(const char *line);

// Movie Parsing Function Prototypes
MovieEntry *build_movie_list(char **lines, int num_lines);
MovieEntry *process_movie_line(const char *line, MovieEntry **movie_list);
void process_actor_line(const char *line, MovieEntry *current_movie);
int is_movie_line(const char *line);
char *extract_movie_title(const char *line);
MovieEntry *create_movie_entry(char *title);
MovieEntry *append_movie_entry(MovieEntry *head, MovieEntry *new_entry);
int add_actor_to_movie(MovieEntry *movie, const char *actor_name);
ActorNameNode *create_actor_name_node(const char *actor_name);
void append_actor_to_movie(MovieEntry *movie, ActorNameNode *new_actor);

// Memory Management Function Prototypes
void free_movie_entry(MovieEntry *movie);
void free_actor_name_list(ActorNameNode *actors);

#endif

