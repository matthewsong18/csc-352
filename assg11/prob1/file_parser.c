/*
 * File: file_parser.c
 * Author: Matthew W. Song
 * Purpose: Provides functions for parsing input files, processing lines,
 * and building movie and actor data structures for the Bacon number
 * calculation.
 */

#include "file_parser.h"

/* ---- Argument Parsing Functions ---- */

/*
 * parse_arguments(argc, argv, long_output, filename)
 * Parses command-line arguments to extract flags and the input file name.
 *
 * Arguments:
 *   argc - Number of command-line arguments.
 *   argv - Array of command-line arguments.
 *   long_output - Pointer to an integer for the -l flag (output mode).
 *   filename - Pointer to store the input file name.
 *
 * Returns:
 *   0 if parsing succeeds, -1 if no valid file name is found.
 */
int parse_arguments(int argc, char *argv[], int *long_output, char **filename) {
  initialize_arguments(long_output, filename);
  return process_command_line_arguments(argc, argv, long_output, filename);
}

/*
 * initialize_arguments(long_output, filename)
 * Initializes the argument flags and filename to default values.
 *
 * Arguments:
 *   long_output - Pointer to the -l flag (output mode, default 0).
 *   filename - Pointer to store the input file name (default NULL).
 */
void initialize_arguments(int *long_output, char **filename) {
  *long_output = 0;
  *filename = NULL;
}

/*
 * process_command_line_arguments(argc, argv, long_output, filename)
 * Processes the command-line arguments to identify the input file and flags.
 *
 * Arguments:
 *   argc - Number of command-line arguments.
 *   argv - Array of command-line arguments.
 *   long_output - Pointer to the -l flag.
 *   filename - Pointer to the input file name.
 *
 * Returns:
 *   0 if a valid file name is found, -1 otherwise.
 */
int process_command_line_arguments(int argc, char *argv[], int *long_output,
                                   char **filename) {
  int file_found = 0;
  for (int i = 1; i < argc; i++) {
    if (is_long_output_flag(argv[i])) {
      *long_output = 1;
    } else if (is_unknown_flag(argv[i])) {
      continue;
    } else {
      if (!file_found) {
        *filename = argv[i];
        file_found = 1;
      }
    }
  }
  return file_found ? 0 : -1;
}

/*
 * is_long_output_flag(arg)
 * Checks if the command-line argument is the -l flag.
 *
 * Arguments:
 *   arg - The command-line argument.
 *
 * Returns:
 *   1 if the argument is -l, 0 otherwise.
 */
int is_long_output_flag(const char *arg) { return strcmp(arg, "-l") == 0; }

/*
 * is_unknown_flag(arg)
 * Checks if the command-line argument is an unknown flag.
 *
 * Arguments:
 *   arg - The command-line argument.
 *
 * Returns:
 *   1 if the argument starts with '-', 0 otherwise.
 */
int is_unknown_flag(const char *arg) { return arg[0] == '-'; }

/* ---- File Reading Functions ---- */

/*
 * read_lines(filename, num_lines)
 * Reads and processes all lines from the input file, filtering out blank lines.
 *
 * Arguments:
 *   filename - Name of the input file.
 *   num_lines - Pointer to store the number of non-blank lines.
 *
 * Returns:
 *   Pointer to an array of strings representing the lines, or NULL on failure.
 */
char **read_lines(const char *filename, int *num_lines) {
  FILE *file_pointer = open_file(filename);
  if (!file_pointer)
    return NULL;

  char **lines = process_file_lines(file_pointer, num_lines);
  close_file(file_pointer);

  return lines;
}

/*
 * open_file(filename)
 * Opens the input file for reading.
 *
 * Arguments:
 *   filename - Name of the input file.
 *
 * Returns:
 *   Pointer to the opened file, or NULL on failure.
 */
FILE *open_file(const char *filename) { return fopen(filename, "r"); }

/*
 * close_file(file_pointer)
 * Closes the opened file.
 *
 * Arguments:
 *   file_pointer - Pointer to the opened file.
 */
void close_file(FILE *file_pointer) { fclose(file_pointer); }

/*
 * process_file_lines(file_pointer, num_lines)
 * Reads lines from the file and filters out blank lines.
 *
 * Arguments:
 *   file_pointer - Pointer to the opened file.
 *   num_lines - Pointer to store the number of non-blank lines.
 *
 * Returns:
 *   Pointer to an array of non-blank lines, or NULL on failure.
 */
char **process_file_lines(FILE *file_pointer, int *num_lines) {
  char **lines = NULL;
  int capacity = 0;
  *num_lines = 0;

  if (read_and_store_lines(file_pointer, &lines, num_lines, &capacity) < 0) {
    free_lines(lines, *num_lines);
    return NULL;
  }
  return lines;
}

/*
 * read_and_store_lines(file_pointer, lines, num_lines, capacity)
 * Reads lines from the file and stores non-blank lines in an array.
 *
 * Arguments:
 *   file_pointer - Pointer to the opened file.
 *   lines - Pointer to the array of lines.
 *   num_lines - Pointer to the number of lines stored.
 *   capacity - Pointer to the capacity of the array.
 *
 * Returns:
 *   0 on success, -1 on memory allocation failure.
 */
int read_and_store_lines(FILE *file_pointer, char ***lines, int *num_lines,
                         int *capacity) {
  char *buffer = NULL;
  size_t buffer_size = 0;

  while (getline(&buffer, &buffer_size, file_pointer) != -1) {
    // Remove trailing newline
    if (buffer[strlen(buffer) - 1] == '\n') {
      buffer[strlen(buffer) - 1] = '\0';
    }

    // Check if line is non-blank
    if (is_non_blank_line(buffer)) {
      if (add_line(lines, num_lines, capacity, buffer) < 0) {
        free(buffer);
        return -1;
      }
    }
  }

  free(buffer);
  return 0;
}

/*
 * add_line(lines, num_lines, capacity, line)
 * Adds a line to the array, reallocating memory as needed.
 *
 * Arguments:
 *   lines - Pointer to the array of lines.
 *   num_lines - Pointer to the current number of lines.
 *   capacity - Pointer to the array capacity.
 *   line - The line to add.
 *
 * Returns:
 *   0 on success, -1 on memory allocation failure.
 */
int add_line(char ***lines, int *num_lines, int *capacity, const char *line) {
  if (*num_lines == *capacity) {
    if (!expand_line_array(lines, capacity)) {
      return -1;
    }
  }

  (*lines)[*num_lines] = strdup(line);
  if (!(*lines)[*num_lines])
    return -1;

  (*num_lines)++;
  return 0;
}

/*
 * expand_line_array(lines, capacity)
 * Expands the capacity of the array of lines.
 *
 * Arguments:
 *   lines - Pointer to the array of lines.
 *   capacity - Pointer to the array capacity.
 *
 * Returns:
 *   1 on success, 0 on memory allocation failure.
 */
int expand_line_array(char ***lines, int *capacity) {
  int new_capacity = (*capacity == 0) ? 4 : (*capacity * 2);
  char **new_lines = realloc(*lines, new_capacity * sizeof(char *));
  if (!new_lines)
    return 0;

  *lines = new_lines;
  *capacity = new_capacity;
  return 1;
}
/*
 * free_lines(lines, num_lines)
 * Frees the memory allocated for an array of strings.
 *
 * Arguments:
 *   lines - Pointer to the array of strings.
 *   num_lines - Number of strings in the array.
 */
void free_lines(char **lines, int num_lines) {
  for (int i = 0; i < num_lines; i++) {
    free(lines[i]);
  }
  free(lines);
}

/* ---- Line Processing Functions ---- */

/*
 * is_non_blank_line(line)
 * Checks if the line is non-blank and contains meaningful characters.
 *
 * Arguments:
 *   line - Pointer to the line string.
 *
 * Returns:
 *   1 if the line is non-blank, 0 otherwise.
 */
int is_non_blank_line(const char *line) {
  return line && strlen(line) > 0 && !is_white_space(line);
}

/*
 * is_white_space(line)
 * Checks if the line contains only whitespace characters.
 *
 * Arguments:
 *   line - Pointer to the line string.
 *
 * Returns:
 *   1 if the line contains only whitespace, 0 otherwise.
 */
int is_white_space(const char *line) {
  for (const char *ptr = line; *ptr; ptr++) {
    if (!isspace(*ptr))
      return 0;
  }
  return 1;
}

/* ---- Movie Parsing Functions ---- */

/*
 * parse_movie_entries(lines, num_lines)
 * Parses an array of lines into a linked list of MovieEntry structures.
 *
 * Arguments:
 *   lines - Array of strings representing the file's lines.
 *   num_lines - Number of lines in the array.
 *
 * Returns:
 *   Pointer to the head of the MovieEntry linked list, or NULL on failure.
 */
MovieEntry *parse_movie_entries(char **lines, int num_lines) {
  return build_movie_list(lines, num_lines);
}

/*
 * build_movie_list(lines, num_lines)
 * Converts the provided lines into a linked list of movies and actors.
 *
 * Arguments:
 *   lines - Array of strings representing the file's lines.
 *   num_lines - Number of lines in the array.
 *
 * Returns:
 *   Pointer to the head of the MovieEntry linked list.
 */
MovieEntry *build_movie_list(char **lines, int num_lines) {
  MovieEntry *movie_list = NULL;
  MovieEntry *current_movie = NULL;

  for (int i = 0; i < num_lines; i++) {
    if (is_movie_line(lines[i])) {
      current_movie = process_movie_line(lines[i], &movie_list);
    } else if (current_movie) {
      process_actor_line(lines[i], current_movie);
    }
  }

  return movie_list;
}

/*
 * process_movie_line(line, movie_list)
 * Processes a movie line and adds a new MovieEntry to the list.
 *
 * Arguments:
 *   line - The movie line to process.
 *   movie_list - Pointer to the head of the MovieEntry linked list.
 *
 * Returns:
 *   Pointer to the newly created MovieEntry.
 */
MovieEntry *process_movie_line(const char *line, MovieEntry **movie_list) {
  char *title = extract_movie_title(line);
  MovieEntry *new_movie = create_movie_entry(title);
  if (!new_movie) {
    free(title);
    return NULL;
  }

  *movie_list = append_movie_entry(*movie_list, new_movie);
  return new_movie;
}

/*
 * process_actor_line(line, current_movie)
 * Processes an actor line and adds it to the current MovieEntry.
 *
 * Arguments:
 *   line - The actor line to process.
 *   current_movie - Pointer to the current MovieEntry.
 */
void process_actor_line(const char *line, MovieEntry *current_movie) {
  add_actor_to_movie(current_movie, line);
}

/*
 * is_movie_line(line)
 * Checks if the line represents a movie.
 *
 * Arguments:
 *   line - The line string.
 *
 * Returns:
 *   1 if the line starts with "Movie: ", 0 otherwise.
 */
int is_movie_line(const char *line) { return strncmp(line, "Movie: ", 7) == 0; }

/*
 * extract_movie_title(line)
 * Extracts the title of a movie from a movie line.
 *
 * Arguments:
 *   line - The movie line string.
 *
 * Returns:
 *   Pointer to a dynamically allocated string containing the movie title.
 */
char *extract_movie_title(const char *line) { return strdup(line + 7); }

/*
 * create_movie_entry(title)
 * Creates a new MovieEntry structure with the given title.
 *
 * Arguments:
 *   title - Pointer to the movie title (dynamically allocated).
 *
 * Returns:
 *   Pointer to the newly created MovieEntry, or NULL on failure.
 */
MovieEntry *create_movie_entry(char *title) {
  MovieEntry *movie = malloc(sizeof(MovieEntry));
  if (movie == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!movie)
    return NULL;

  movie->title = title;
  movie->actors = NULL;
  movie->next = NULL;
  return movie;
}

/*
 * append_movie_entry(head, new_entry)
 * Appends a new MovieEntry to the end of a linked list of MovieEntry
 * structures.
 *
 * Arguments:
 *   head - Pointer to the head of the MovieEntry linked list.
 *   new_entry - Pointer to the new MovieEntry to append.
 *
 * Returns:
 *   Pointer to the head of the linked list.
 */
MovieEntry *append_movie_entry(MovieEntry *head, MovieEntry *new_entry) {
  if (!head)
    return new_entry;

  MovieEntry *current = head;
  while (current->next) {
    current = current->next;
  }
  current->next = new_entry;
  return head;
}

/*
 * add_actor_to_movie(movie, actor_name)
 * Adds a new actor to the given MovieEntry's list of actors.
 *
 * Arguments:
 *   movie - Pointer to the MovieEntry structure.
 *   actor_name - Name of the actor to add (dynamically allocated).
 *
 * Returns:
 *   0 on success, -1 on memory allocation failure.
 */
int add_actor_to_movie(MovieEntry *movie, const char *actor_name) {
  ActorNameNode *actor = create_actor_name_node(actor_name);
  if (!actor)
    return -1;

  append_actor_to_movie(movie, actor);
  return 0;
}

/*
 * create_actor_name_node(actor_name)
 * Creates a new ActorNameNode structure for the given actor name.
 *
 * Arguments:
 *   actor_name - Name of the actor (dynamically allocated).
 *
 * Returns:
 *   Pointer to the newly created ActorNameNode, or NULL on failure.
 */
ActorNameNode *create_actor_name_node(const char *actor_name) {
  ActorNameNode *actor = malloc(sizeof(ActorNameNode));
  if (actor == NULL) {
    fprintf(stderr, "ERROR: RAN OUT OF MEMORY\n");
    exit(1);
  }
  if (!actor)
    return NULL;

  actor->name = strdup(actor_name);
  if (!actor->name) {
    free(actor);
    return NULL;
  }
  actor->next = NULL;
  return actor;
}

/*
 * append_actor_to_movie(movie, new_actor)
 * Appends a new actor to a MovieEntry's list of actors.
 *
 * Arguments:
 *   movie - Pointer to the MovieEntry structure.
 *   new_actor - Pointer to the ActorNameNode to append.
 */
void append_actor_to_movie(MovieEntry *movie, ActorNameNode *new_actor) {
  if (!movie->actors) {
    movie->actors = new_actor;
  } else {
    ActorNameNode *current = movie->actors;
    while (current->next) {
      current = current->next;
    }
    current->next = new_actor;
  }
}

/* ---- Memory Management Functions ---- */

/*
 * free_movie_entry(movie)
 * Frees the memory allocated for a single MovieEntry structure.
 *
 * Arguments:
 *   movie - Pointer to the MovieEntry structure.
 */
void free_movie_entry(MovieEntry *movie) {
  free(movie->title);
  free_actor_name_list(movie->actors);
  free(movie);
}

/*
 * free_actor_name_list(actors)
 * Frees the memory allocated for a linked list of ActorNameNode structures.
 *
 * Arguments:
 *   actors - Pointer to the head of the ActorNameNode linked list.
 */
void free_actor_name_list(ActorNameNode *actors) {
  while (actors) {
    ActorNameNode *next = actors->next;
    free(actors->name);
    free(actors);
    actors = next;
  }
}

/*
 * free_movie_entry_list(movie_list)
 * Frees the memory allocated for a linked list of MovieEntry structures.
 *
 * Arguments:
 *   movie_list - Pointer to the head of the MovieEntry linked list.
 */
void free_movie_entry_list(MovieEntry *movie_list) {
  while (movie_list) {
    MovieEntry *next = movie_list->next;
    free_movie_entry(movie_list);
    movie_list = next;
  }
}
