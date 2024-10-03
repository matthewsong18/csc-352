/*
 * File: anagrams.c
 * Author: Matthew W. Song
 * Purpose: Identifies and prints out anagrams from a set of strings. The first value is the reference that the
 * program compares against every string that follows after it. The reference and every string that matches as an
 * anagram will be printed.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {

    char inputString[64];
    char referenceString[64] = {'\0'};
    int errorStatus = 0;

    int input;
    while ((input = scanf("%63s", inputString)) > 0) {
        int alpha = 1;
        for (int i = 0; i < strlen(inputString); i++) {

            // Check that everything is alphabetic
            if (!isalpha(inputString[i])) {
                fprintf(stderr, "Error: Non-alphabetical was found\n");
                errorStatus = 1;
                alpha = 0;

                // End if the first string or reference string is unqualified
                if (strlen(referenceString) == 0) return 1;

                else break;
            }

        }

        // Skip the string if non-alphabetic is found
        if (alpha != 1) continue;

        // Set the reference string or skip if already set
        if (strlen(referenceString) == 0) {
            strcpy(referenceString, inputString);
            printf("%s\n", referenceString);
            continue;
        }

        // Just skip if they don't match lengths
        if (strlen(referenceString) != strlen(inputString)) {
            continue;
        }

        // Go through each letter of the reference and find matches
        for (int i = 0; i < strlen(referenceString); i++) {
            char letter = tolower(referenceString[i]);

            int letterCount = 0;

            // Count how many times the letter appears
            for (int p = 0; p < strlen(referenceString); p++) {
                if (letter == tolower(referenceString[p])) letterCount++;
            }

            // Count backwards how many times the letter is found
            for (int n = 0; n < strlen(inputString); n++) {
                if (letter == tolower(inputString[n])) {
                    letterCount--;
                }
            }

            // If they both have the letter the same number of times, the count should be 0
            if (letterCount != 0) {
                break;
            }
        }

        printf("%s\n", inputString);
    }

    if (input != -1) {
        fprintf(stderr, "Error: The standard input buffer exited before hitting EOF");
        return 1;
    }

    fflush(stdout);

    return errorStatus;
}
