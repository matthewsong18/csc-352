#ifndef SAFETY_H
#define SAFETY_H

#include <stddef.h>

extern int allocation_count; // Extern declaration

void handle_error(const char *message);
void *safe_malloc(size_t size);
void *safe_strdup(const char *string);
void safe_free(void **ptr);
void check_for_leaks();
int check_allocation_count();
void reset_allocation_count();

#endif // SAFETY_H

