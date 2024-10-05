/*
 * File: strmath.c
 * Author: Matthew W Song
 * Purpose: do math
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void inputValidation(int, char **);

void performMath(char **);

void isWhitespace(char *);

void isNonNumeric(char *);

void hasLeadingZero(char *);

int max(int, int);

int getDigit(int);

int math(int, int, int);

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

    ++lineNum;

    if (lineNum == 3)
      break;
  }

  inputValidation(lineNum, stringList);

  performMath(stringList);

  fflush(stdout);

  free(line);

  free(stringList[0]);
  free(stringList[1]);
  free(stringList[2]);

  exit(0);
}

void inputValidation(int lineNum, char **stringList) {

  if (lineNum != 3) {
    fprintf(stderr, "Not enough input strings\n");
    exit(1);
  }

  char *firstString = stringList[0];

  int notAdd = strcmp(firstString, "add") != 0;
  int notSub = strcmp(firstString, "sub") != 0;

  if (notAdd && notSub) {
    fprintf(stderr, "First string is not one of \"add\" or \"sub\".\n");
    exit(1);
  }

  isWhitespace(stringList[1]);
  isWhitespace(stringList[2]);
  isNonNumeric(stringList[1]);
  isNonNumeric(stringList[2]);
  hasLeadingZero(stringList[1]);
  hasLeadingZero(stringList[2]);
}

void performMath(char **stringList) {
  int isAdd = strcmp(stringList[0], "add") == 0;
  char *string1 = stringList[1];
  char *string2 = stringList[2];

  // Max possible length is one more than longer number
  unsigned long lenStr1 = strlen(string1);
  unsigned long lenStr2 = strlen(string2);

  int mathStrLen = max(lenStr1, lenStr2) + 1;
  char *mathStr = calloc(mathStrLen, sizeof(int));

  if (mathStr == NULL) {
    fprintf(stderr, "Out of memory\n");
    exit(1);
  }

  int usableSpace = mathStrLen;

  for (int i = 1; i < usableSpace; ++i) {
    int num1 = 0;
    int num2 = 0;

    if (lenStr1 >= i) {
      num1 = getDigit(string1[lenStr1 - i]);
    }
    if (lenStr2 >= i) {
      num2 = getDigit(string2[lenStr2 - i]);
    }

    mathStr[usableSpace - i] =
        mathStr[usableSpace - i] + math(isAdd, num1, num2);

    if (mathStr[usableSpace - i] >= 10) {
      mathStr[usableSpace - i] = mathStr[usableSpace - i] % 10;
      mathStr[usableSpace - (i + 1)] = 1;
    }
  }

  int unset = 1;
  int negative = 0;
  for (int i = 0; i < mathStrLen; ++i) {

    // Setting positive or negative mode
    if (unset) {
      if (mathStr[i] == 0)
        continue;
      else {
        unset = 0;
        if (mathStr[i] < 0)
          negative = 1;
        i = -1;
        continue;
      }
    }

    if (mathStr[i] == 0) {
      continue;
    }

    if (negative) {
      if (mathStr[i] > 0) {
        mathStr[i - 1] += 1;
        mathStr[i] = -10 + mathStr[i];
        i -= 2;
      }
      continue;
    }
    // positive mode
    else {
      if (mathStr[i] < 0) {
        mathStr[i - 1] -= 1;
        mathStr[i] = 10 + mathStr[i];
        i -= 2;
      }
      continue;
    }
  }

  int leadingZero = 1;
  int leadingNeg = 1;
  for (int i = 0; i < mathStrLen; ++i) {
    if (mathStr[i] != 0)
      leadingZero = 0;
    if (leadingZero) {
      continue;
    }
    if (negative && !leadingNeg) {
      printf("%d", mathStr[i] * -1);
    } else {
      printf("%d", mathStr[i]);
      leadingNeg = 0;
    }
  }
  printf("\n");
  fflush(stdout);

  free(mathStr);
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

void isNonNumeric(char *line) {
  for (int i = 0; line[i] != '\0'; ++i) {

    if (isdigit(line[i]) == 0) {
      fprintf(stderr, "Error: Non-integer characters in input\n");
      exit(1);
    }
  }
}

void hasLeadingZero(char *line) {
  if (line[0] == 0 && strlen(line) != 1) {
    fprintf(stderr, "Leading zeros are not allowed in numbers.\n");
    exit(1);
  }
}

int max(int num1, int num2) {
  if (num1 > num2)
    return num1;
  else if (num2 > num1)
    return num2;
  return num1;
}

int getDigit(int digit) { return digit - '0'; }

int math(int isAdd, int num1, int num2) {
  if (isAdd) {
    return num1 + num2;
  } else {
    return num1 - num2;
  }
}
