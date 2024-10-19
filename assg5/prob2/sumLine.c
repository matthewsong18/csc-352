#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int errorStatus = 0;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  // Read in line
  while ((linelen = getline(&line, &linecap, stdin)) > 0) {
    int retval;
    int number;
    int sum = 0;

    // Parse line for numbers
    int lineShift = 0;
    int shiftAmount;
    while ((retval = sscanf(&line[lineShift], "%d %n", &number, &shiftAmount)) >
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
