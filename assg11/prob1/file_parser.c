#include "file_parser.h"

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
