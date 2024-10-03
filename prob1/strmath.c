/*
 * File: strmath.c
 * Author: Matthew W Song
 * Purpose: do math
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Input is 3 strings, one per line
    char *line = NULL;
    size_t linecap = 0;
    int lineNum = 0;

    while (getline(&line, &linecap, stdin) > 0) {
        ++lineNum;
        printf("Line: %s", line);
    }

    free(line);

    return 0;
}
