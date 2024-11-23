/*
 * File: safety.c
 * Author: Matthew
 * Purpose: Provides safe memory management utilities and error handling for the
 * program. Includes wrappers for common memory operations, a memory leak
 * checker, and error-handling functions.
 */

#include "safety.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int allocation_count = 0; // Tracks the number of active memory allocations

/*
 * handle_error(message) -- Prints an error message and exits the program.
 *
 * Arguments:
 *    message - The error message to display (must not be NULL).
 *
 * Behavior:
 *    - Prints the error message to stderr using perror().
 *    - Exits the program with status 1.
 */
void handle_error(const char *message) {
  perror(message);
  exit(1);
}

/*
 * safe_malloc(size) -- Allocates memory and checks for allocation errors.
 *
 * Arguments:
 *    size - The size of memory to allocate (in bytes).
 *
 * Returns:
 *    A pointer to the allocated memory.
 *
 * Behavior:
 *    - Uses malloc to allocate memory.
 *    - If allocation fails, calls handle_error() to terminate the program.
 *    - Increments the allocation count for leak tracking.
 */
void *safe_malloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    handle_error("Memory allocation failed");
  }
  allocation_count++;
  return ptr;
}

/*
 * safe_strdup(string) -- Duplicates a string and checks for allocation errors.
 *
 * Arguments:
 *    string - The string to duplicate (must not be NULL).
 *
 * Returns:
 *    A pointer to the duplicated string.
 *
 * Behavior:
 *    - Uses strdup to duplicate the string.
 *    - If duplication fails, calls handle_error() to terminate the program.
 *    - Increments the allocation count for leak tracking.
 */
void *safe_strdup(const char *string) {
  void *ptr = strdup(string);
  if (!ptr) {
    handle_error("Memory allocation failed");
  }
  allocation_count++;
  return ptr;
}

/*
 * safe_free(ptr) -- Frees allocated memory and nullifies the pointer.
 *
 * Arguments:
 *    ptr - A double pointer to the memory to free (must not be NULL).
 *
 * Behavior:
 *    - Frees the memory pointed to by *ptr.
 *    - Sets *ptr to NULL to avoid dangling pointers.
 *    - Decrements the allocation count for leak tracking.
 *    - Prints a warning if called with a NULL or already freed pointer.
 */
void safe_free(void **ptr) {
  if (ptr && *ptr) {
    allocation_count--;
    free(*ptr);
    *ptr = NULL; // Nullify the pointer in the caller's scope
  } else {
    fprintf(stdout, "Warning: Attempt to free null or already freed memory\n");
  }
}

/*
 * check_for_leaks() -- Checks if there are any memory leaks.
 *
 * Behavior:
 *    - Prints the number of allocations not freed if any memory leaks are
 * detected.
 *    - Prints a message confirming no leaks if all memory is properly freed.
 */
void check_for_leaks() {
  if (allocation_count != 0) {
    fprintf(stdout, "Memory leak detected: %d allocations not freed\n",
            allocation_count);
  } else {
    printf("No memory leaks detected.\n");
  }
}

/*
 * check_allocation_count() -- Returns the current number of active allocations.
 *
 * Returns:
 *    The number of active allocations.
 */
int check_allocation_count() { return allocation_count; }

/*
 * reset_allocation_count() -- Resets the allocation count to zero.
 *
 * Behavior:
 *    - Sets the allocation_count variable to 0.
 *    - Intended for use in testing scenarios where allocations are simulated.
 */
void reset_allocation_count() { allocation_count = 0; }
