#include "file_parser.h"
#include <stdio.h>

int parse_arguments(int argc, char *argv[], int *long_output, char **filename) {
  int file_found = 0;
  *long_output = 0;
  *filename = NULL;

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-l") == 0) {
      // Found a long_output flag
      *long_output = 1;
    } else if (argv[i][0] == '-') {
      // Unknown flag: ignore or handle error if desired
      // For now, we just ignore unknown flags
    } else {
      // Assume this is the filename
      if (!file_found) {
        *filename = argv[i];
        file_found = 1;
      }
    }
  }

  // If no filename was provided, return error
  if (!file_found) {
    return -1;
  }

  return 0;
}

// Adds a line to the array, handling reallocation if needed
static int add_line_to_array(char ***lines, int *num_lines, int *capacity,
                             const char *line) {
  if (*num_lines == *capacity) {
    size_t new_capacity = (*capacity == 0) ? 4 : (*capacity * 2);
    char **new_lines = realloc(*lines, new_capacity * sizeof(*new_lines));
    if (!new_lines) {
      return -1; // Allocation failed
    }
    *lines = new_lines;
    *capacity = (int)new_capacity;
  }

  char *line_copy = strdup(line);
  if (!line_copy) {
    return -1; // Allocation failed
  }

  (*lines)[*num_lines] = line_copy;
  (*num_lines)++;
  return 0;
}

// Reads all lines from the file, filtering out blank lines, and stores them in
// an array.
static int read_and_process_lines(FILE *fp, char ***lines, int *num_lines) {
  char *buffer = NULL;
  size_t buf_size = 0;
  ssize_t len;

  int capacity = 0;
  *lines = NULL;
  *num_lines = 0;

  while ((len = getline(&buffer, &buf_size, fp)) != -1) {
    // Remove trailing newline if present
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }

    // Check if line is non-blank
    if (!is_blank_line(buffer)) {
      if (add_line_to_array(lines, num_lines, &capacity, buffer) < 0) {
        // Cleanup on allocation failure
        for (int i = 0; i < *num_lines; i++) {
          free((*lines)[i]);
        }
        free(*lines);
        *lines = NULL;
        *num_lines = 0;

        free(buffer);
        return -1;
      }
    }
  }

  free(buffer);
  return 0; // Success
}

char **read_lines(const char *filename, int *num_lines) {
  *num_lines = 0;
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    return NULL; // Could not open file
  }

  char **lines = NULL;
  if (read_and_process_lines(fp, &lines, num_lines) < 0) {
    // read_and_process_lines handled cleanup in case of error
    fclose(fp);
    return NULL;
  }

  fclose(fp);
  return lines;
}

static int is_movie_line(const char *line) {
  // Checks if the line starts with "Movie: "
  return strncmp(line, "Movie: ", 7) == 0;
}

static char *extract_movie_title(const char *line) {
  // Duplicate the substring after "Movie: "
  return strdup(line + 7);
}

static MovieEntry *create_movie_entry(char *title) {
  MovieEntry *m = malloc(sizeof(MovieEntry));
  if (!m) {
    free(title);
    return NULL;
  }
  m->title = title;
  m->actors = NULL;
  m->next = NULL;
  return m;
}

static ActorNameNode *create_actor_name_node(const char *actor_name) {
  ActorNameNode *a = malloc(sizeof(ActorNameNode));
  if (!a)
    return NULL;
  a->name = strdup(actor_name);
  if (!a->name) {
    free(a);
    return NULL;
  }
  a->next = NULL;
  return a;
}

static int add_actor_to_movie(MovieEntry *movie, const char *actor_name) {
  ActorNameNode *a = create_actor_name_node(actor_name);
  if (!a)
    return -1;
  if (!movie->actors) {
    movie->actors = a;
  } else {
    ActorNameNode *cur = movie->actors;
    while (cur->next)
      cur = cur->next;
    cur->next = a;
  }
  return 0;
}

static MovieEntry *append_movie_entry(MovieEntry *head, MovieEntry *new_entry) {
  if (!head)
    return new_entry;
  MovieEntry *cur = head;
  while (cur->next)
    cur = cur->next;
  cur->next = new_entry;
  return head;
}

// Cleanup function for error handling
void free_movie_list(MovieEntry *head) {
  while (head) {
    MovieEntry *nm = head->next;
    free(head->title);
    ActorNameNode *a = head->actors;
    while (a) {
      ActorNameNode *na = a->next;
      free(a->name);
      free(a);
      a = na;
    }
    free(head);
    head = nm;
  }
}

MovieEntry *parse_movie_entries(char **lines, int num_lines) {
  MovieEntry *movie_list = NULL, *current_movie = NULL;

  for (int i = 0; i < num_lines; i++) {
    if (is_movie_line(lines[i])) {
      char *title = extract_movie_title(lines[i]);
      if (!title) {
        free_movie_list(movie_list);
        return NULL;
      }
      MovieEntry *m = create_movie_entry(title);
      if (!m) {
        free_movie_list(movie_list);
        return NULL;
      }
      movie_list = append_movie_entry(movie_list, m);
      current_movie = m;
    } else {
      if (!current_movie)
        continue; // Ignore actors before first movie line
      if (add_actor_to_movie(current_movie, lines[i]) < 0) {
        free_movie_list(movie_list);
        return NULL;
      }
    }
  }
  return movie_list;
}

int is_blank_line(const char *line) {
  if (line == NULL)
    return 1;
  if (strlen(line) <= 0)
    return 1;
  if (is_white_space(line))
    return 1;
  return 0;
}

int is_white_space(const char *line) {
  if (line == NULL)
    exit(1);

  char *buffer = strdup(line);
  char *buffer_ptr = buffer;
  while (*buffer != '\0') {
    const int is_space = isspace(*buffer);
    if (is_space == 0) {
      free(buffer_ptr);
      return 0;
    }
    buffer++;
  }
  free(buffer_ptr);
  return 1;
}

// // Frees a linked list of MovieEntry objects
// void free_movie_list(MovieEntry *movie_list) {
//   while (movie_list) {
//     MovieEntry *next_movie = movie_list->next;

//     // Free the movie title
//     free(movie_list->title);

//     // Free the linked list of actors
//     ActorNameNode *actor = movie_list->actors;
//     while (actor) {
//       ActorNameNode *next_actor = actor->next;
//       free(actor->name);
//       free(actor);
//       actor = next_actor;
//     }

//     // Free the MovieEntry itself
//     free(movie_list);

//     // Move to the next movie
//     movie_list = next_movie;
//   }
// }
