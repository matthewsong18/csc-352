/*
 * File: changeBase.c
 * Author: Matthew W. Song
 * Purpose: This program will convert strings representing numbers into a different base. It accepts a base number
 * from 2 to 36 and strings representing numbers where letters 'A' to 'Z' represent 10 to 35. There cannot be a value
 * inside the string that is equivalent to a value greater than the base. The converted numbers will be printed.
 */

#include <string.h>
#include <ctype.h>
#include "stdio.h"

void checkForSymbols(const char *inputString, int *errorStatus, int *skipStatus);

int main(void) {
    int errorStatus = 0;

    int BASE_MIN = 2;
    int BASE_MAX = 36;

    // Subtracting the char of an uppercase letter by the ASCII_ADJUSTMENT will obtain an int from 10 to 35.
    int ASCII_ADJUSTMENT = 55;
    char inputString[7];

    int input;
    int firstInput;
    int base = 0;

    while ((input = scanf("%6s", inputString)) > 0) {
        int skipStatus = 0;

        if (strlen(inputString) == 0 && base == 0) {
            fprintf(stderr, "Error: Input is empty\n");
            return 1;
        }

        checkForSymbols(inputString, &errorStatus, &skipStatus);

        if (skipStatus) continue;

        // Assign base number
        if (base == 0) {
            if (!firstInput) {
                fprintf(stderr, "Error: Base is somehow empty after the first input\n");
                return 1;
            }
            firstInput = 0;

            int x = 0;

            for (int i = 0; inputString[i] != '\0'; i++) {
                // Honer's Rule with base 10
                x = 10 * x + (inputString[i] - '0');
            }

            // Base qualification check
            if (x < BASE_MIN || x > BASE_MAX) {
                fprintf(stderr, "Error: Base is out of legal range\n");
                return 1;
            }

            base = x;
            continue;
        }

        unsigned long result = 0;

        for (int i = 0; inputString[i] != '\0'; ++i) {
            int inputNumber = inputString[i] - '0';

            // If alphabetic, adjust for ASCII
            if (isalpha(inputString[i]) != 0) {
                inputNumber = toupper(inputString[i]) - ASCII_ADJUSTMENT;
            }

            // Checking if inputNumber is within limit of base
            if (inputNumber >= base) {
                fprintf(stderr, "Error: Not a %d base number\n", base);
                errorStatus = 1;
                skipStatus = 1;
                break;
            }

            // Honer's rule
            result = base * result + inputNumber;
        }

        if (skipStatus) continue;

        printf("%lu\n", result);
        fflush(stdout);
    }

    if (input != -1) {
        fprintf(stderr, "Error: The standard input buffer exited before hitting EOF");
        return 1;
    }

    return errorStatus;
}

// Check if symbol is detected and change statuses accordingly
void checkForSymbols(const char *inputString, int *errorStatus, int *skipStatus) {
    for (int i = 0; inputString[i] != '\0'; ++i) {
        char inputChar = inputString[i];
        if (isalnum(inputChar) == 0) {
            (*errorStatus) = 1;
            fprintf(stderr, "Error: Symbols are not allowed\n");
            (*skipStatus) = 1;
            break;
        }
    }
}
