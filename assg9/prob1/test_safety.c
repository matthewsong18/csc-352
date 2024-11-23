#include "safety.h"
#include <stdio.h>

// prototypes for tests
int test_safe_malloc();
int test_safe_free();
int test_check_for_leaks();

void run_test(const char *test_name, int (*test_func)()) {
  printf("Running %s...\n", test_name);
  int result = test_func();
  if (result) {
    printf("Result: %s passed!\n\n", test_name);
  } else {
    printf("Result: %s failed.\n\n", test_name);
  }
}

int main() {
  int total_tests = 3, passed_tests = 0;

  printf("Starting Tests...\n");
  if (test_safe_malloc())
    passed_tests++;
  if (test_safe_free())
    passed_tests++;
  if (test_check_for_leaks())
    passed_tests++;

  printf("Test Summary: %d/%d tests passed.\n", passed_tests, total_tests);
  return (total_tests == passed_tests)
             ? 0
             : 1; // Exit with failure if not all tests pass
}

// test cases

int test_safe_malloc() {
  int success = 1;

  printf("Initial allocation count: %d\n", allocation_count);

  void *ptr1 = safe_malloc(128);
  void *ptr2 = safe_malloc(64);

  if (allocation_count != 2) {
    printf("Error: Allocation count mismatch. Expected 2, got %d\n",
           allocation_count);
    success = 0;
  }

  safe_free(&ptr1);
  safe_free(&ptr2);

  if (allocation_count != 0) {
    printf(
        "Error: Allocation count mismatch after freeing. Expected 0, got %d\n",
        allocation_count);
    success = 0;
  }

  return success;
}

int test_safe_free() {
  int success = 1;

  void *ptr = safe_malloc(50);
  safe_free(&ptr);

  if (ptr != NULL) {
    printf("Error: Pointer not NULL after free.\n");
    success = 0;
  }

  if (allocation_count != 0) {
    printf("Error: Allocation count mismatch after free.\n");
    success = 0;
  }

  return success;
}

int test_check_for_leaks() {
  int success = 1;

  void *ptr = safe_malloc(100);
  if (allocation_count != 1) {
    printf("Error: Allocation count mismatch. Expected 1, got %d\n",
           allocation_count);
    success = 0;
  }

  check_for_leaks(); // Should report a leak

  safe_free(&ptr);
  if (allocation_count != 0) {
    printf("Error: Allocation count mismatch after free. Expected 0, got %d\n",
           allocation_count);
    success = 0;
  }

  check_for_leaks(); // Should report no leaks
  return success;
}
