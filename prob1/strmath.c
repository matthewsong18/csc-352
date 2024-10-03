/*
 * File: strmath.c
 * Author: Matthew W Song
 * Purpose: do math
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  // Input is 3 strings, one per line
  char *stringList[3];

  char *line = NULL;
  size_t linecap = 0;
  int lineNum = 0;

  while (getline(&line, &linecap, stdin) != EOF) {
    printf("%lu", strlen(line));
    stringList[lineNum] = malloc(strlen(line) + 1);
    if (stringList[lineNum] == NULL) {
      fprintf(stderr, "Out of memory\n");
      exit(1);
    }

    strcpy(stringList[lineNum], line);

    ++lineNum;
    printf("Line: %s", line);

    if (lineNum == 3)
      break;
  }

  if (lineNum != 3) {
    fprintf(stderr, "Not enough input strings");
    exit(1);
  }

  char *firstString = stringList[0];

  int notAdd = !strcmp(firstString, "add");
  int notSub = !strcmp(firstString, "sub");
  printf("%s\n", firstString);
  printf("%d\n", notAdd);
  printf("%d\n", notSub);
  
  if (notAdd || notSub) {
    fprintf(stderr, "First string is not one of \"add\" or \"sub\".");
    exit(1);
  }

  free(line);

  return 0;
}
