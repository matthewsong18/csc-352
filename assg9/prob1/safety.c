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
  if (ptr && *ptr) {
    allocation_count--;
    free(*ptr);
    *ptr = NULL; // Nullify the pointer in the caller's scope
  } else {
    fprintf(stdout, "Warning: Attempt to free null or already freed memory\n");
  }
}

// Check for memory leaks
void check_for_leaks() {
  if (allocation_count != 0) {
    fprintf(stdout, "Memory leak detected: %d allocations not freed\n",
            allocation_count);
  } else {
    printf("No memory leaks detected.\n");
  }
}

// Get allocation count
int check_allocation_count() { return allocation_count; }

// Reset allocation count (for testing purposes)
void reset_allocation_count() { allocation_count = 0; }
