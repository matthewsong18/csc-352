/*
 * FIle: rotate.c
 * Author: Matthew W Song
 * Purpose: Counts how many times each integer appears in a sequence of
 * integers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    int n = 0;

    int *input_values, buffer;

    // Check if we can read at least one integer from stdin
    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Cannot read at least one integer from stdin\n");
        exit(1);
    }

    // Check if the first integer value is positive
    if (n <= 0) {
        fprintf(stderr, "The first integer value is not positive\n");
        exit(1);
    }

    input_values = malloc(n * sizeof(int));
    if (input_values == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    // Read input values
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &buffer) < 0) {
            break;
        }
        input_values[i] = buffer;

        input_values[i] = buffer;
        count++;
    }

    // Check if fewer integers are available than indicated
    if (count < n) {
        fprintf(stderr, "Fewer integers available to read than indicated\n");
        exit(1);
    }

    int r;
    // Check if we can read at least one integer from stdin
    if (scanf("%d", &r) != 1) {
        fprintf(stderr, "Cannot read at least one integer from stdin\n");
        exit(1);
    }

    int *returnList = (int*) malloc(n * sizeof(int));
    if (r > 0) {
        r = r % n;
        for (int i = 0; i < n; ++i) {
            int newIndex = i + r;
            if (newIndex >= n) {
                newIndex -= n;
            }
            returnList[newIndex] = input_values[i];
        }
    } else if (r < 0) {
        r = r % n;
        for (int i = 0; i < n; ++i) {
            int newIndex = i + r;
            if (newIndex < 0) {
                newIndex += n;
            }
            returnList[newIndex] = input_values[i];
        }
    }

    for (int i = 0; i < n; ++i) {
        printf("%d ", returnList[i]);
    }
    printf("\n");

    free(input_values);
    free(returnList);
    return 0;
}