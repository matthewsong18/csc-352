#include "safety.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int allocation_count = 0;

// Handle errors and exit the program
void handle_error(const char *message) {
  perror(message);
  exit(1);
}

// Safe malloc wrapper
void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    handle_error("Memory allocation failed");
  }
  allocation_count++;
  return ptr;
}

// Safe strdup wrapper
void *safe_strdup(const char *string) {
  void *ptr = strdup(string);
  if (!ptr) {
    handle_error("Memory allocation failed");
  }
  allocation_count++;
  return ptr;
}

// Free allocated memory
void safe_free(void **ptr) {
  if (*ptr) {
    allocation_count--;
    free(*ptr);
    *ptr = NULL;
  }
}

void check_for_leaks() {
  if (allocation_count != 0) {
    fprintf(stderr, "Memory leak detected: %d allocations not freed\n",
            allocation_count);
  }
}
