/*
 * File: sumLine.c
 * Author: Matthew W Song
 * Purpose: A program that gathers a number of integers that are positive and
 * add them together into a sum for each line. Characters that are not white
 * space or a number are not allowed.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int errorStatus = 0;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  int skipLine = 0;

  // Read in line
  while ((linelen = getline(&line, &linecap, stdin)) > 0) {

    // Validate that the line contains only digits and spaces
    for (int i = 0; i < linelen && line[i] != '\n'; ++i) {
      if (!isdigit(line[i]) && !isspace(line[i])) {
        errorStatus = 1;
        skipLine = 1;
        break;
      }
    }

    if (skipLine) {
      fprintf(stderr, "Bad line input\n");
      skipLine = 0;
      continue;
    }

    int retval;
    int number;
    int sum = 0;

    // Parse line for numbers
    int lineShift = 0;
    int shiftAmount;
    while ((retval = sscanf(&line[lineShift], "%d%n", &number, &shiftAmount)) >
           0) {

      if (number < 0) {
        errorStatus = 1;
        break;
      }

      sum += number;
      lineShift += shiftAmount;
    }

    if (retval != -1) {
      fprintf(stderr, "Bad line input\n");
      errorStatus = 1;
    } else {
      // Get sum of line
      printf("%d\n", sum);
    }
  }

  fflush(stdout);

  return errorStatus;
}
