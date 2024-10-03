/*
 * File: strmath.c
 * Author: Matthew W Song
 * Purpose: do math
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void isWhitespace(char *);

void inputValidation(int, char **);

int main(void) {
  // Input is 3 strings, one per line
  char *stringList[3];

  char *line = NULL;
  size_t linecap = 0;
  int lineNum = 0;

  while (getline(&line, &linecap, stdin) != EOF) {
    stringList[lineNum] = malloc(strlen(line) + 1);
    if (stringList[lineNum] == NULL) {
      fprintf(stderr, "Out of memory\n");
      exit(1);
    }

    strcpy(stringList[lineNum], line);
    stringList[lineNum][strcspn(stringList[lineNum], "\n")] = 0;

    printf("%s", stringList[lineNum]);

    ++lineNum;

    if (lineNum == 3)
      break;
  }

  inputValidation(lineNum, stringList);

  free(line);

  return 0;
}

void inputValidation(int lineNum, char **stringList) {

  if (lineNum != 3) {
    fprintf(stderr, "Not enough input strings\n");
    exit(1);
  }

  char *firstString = stringList[0];

  int notAdd = strcmp(firstString, "add") != 0;
  int notSub = strcmp(firstString, "sub") != 0;

  if (notAdd || notSub) {
    fprintf(stderr, "First string is not one of \"add\" or \"sub\".\n");
    exit(1);
  }
  isWhitespace(stringList[1]);
  isWhitespace(stringList[2]);
}

void isWhitespace(char *line) {
  int isEmpty = strlen(line) == 0;

  if (isEmpty) {
    fprintf(stderr, "A line contains only whitespace.\n");
    exit(1);
  }

  for (int i = 0; line[i] != '\0'; ++i) {
    if (isspace(line[i])) {
      fprintf(stderr, "A line contains only whitespace.\n");
      exit(1);
    }
  }
}
