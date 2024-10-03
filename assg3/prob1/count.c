/*
 * FIle: count.c
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

    // Sort values

    for (int i = 0; i < n; ++i) {
        int minimum = input_values[i];
        int minimumIndex = i;
        for (int j = i + 1; j < n; ++j) {
            if (input_values[j] < minimum) {
                minimum = input_values[j];
                minimumIndex = j;
            }
        }
        int temp = input_values[i];
        input_values[i] = minimum;
        input_values[minimumIndex] = temp;
    }

    for (int i = 0; i < n; ++i) {
        printf("%d ", input_values[i]);
    }
    printf("\n");


    // Count values
    int *pastNums = (int *) malloc(n * sizeof(int));
    int uniqueNumbers = 0;

    for (int i = 0; i < n; ++i) {
        int numCount = 0;
        for (int j = 0; j < n; ++j) {
            if (input_values[i] == input_values[j]) ++numCount;
        }
        if (uniqueNumbers == 0 || pastNums[uniqueNumbers - 1] != input_values[i]) {
            pastNums[uniqueNumbers] = input_values[i];
            ++uniqueNumbers;
            printf("%d %d\n", input_values[i], numCount);
        }
    }

    free(input_values);
    free(pastNums);

    return 0;
}