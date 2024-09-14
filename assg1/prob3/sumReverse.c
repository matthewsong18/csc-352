/*
 * File: sumReverse.c
 * Author: Matthew W. Song
 * Purpose: The program reads in a sequence of positive integers until no more can be read in. For each positive
 * integer, the integer is summed with the reverse of the integer and printed out.
*/

#include <stdio.h>
#include "math.h"

/*
 * Reverse the positive integer that is passed in by counting how many digits the integer has, then creating a new
 * integer by multiplying the digits of the integer from back to front by 10 to the power of (digit count - 1),
 * subtracting 1 for each digit added to the reverse until the reverse integer is formed.
 *
 * Returns: 1 if an error occurs. 0 otherwise.
 */
int reverseInteger(int number) {
    if (number <= 0) return -1;

    // Count digits
    int copyNumber = number;
    int digitCount = 1;

    while (copyNumber >= 10) {
        copyNumber /= 10;
        digitCount++;
    }

    // Create new reverse integer
    copyNumber = number;
    int reverseNumber = 0;

    for (int i = digitCount; i > 0; i--) {
        reverseNumber += (copyNumber % 10) * (int) pow(10, i - 1);
        copyNumber /= 10;
    }

    return reverseNumber;
}

/*
 * Read in a sequence of positive integers and calculate the sum of each integer with their reverse.
 *
 * Returns: 1 if an error occurs. 0 otherwise.
 */
int main(void) {
    int targetNumber;

    int input = scanf("%d", &targetNumber);

    // Greater than 0 should end on non-integers and EOF
    while (input > 0) {
        if (targetNumber < 0) {
            fprintf(stderr, "Error: not a positive integer\n");
        }

        int reverseNumber = reverseInteger(targetNumber);
        if (reverseNumber == -1) return 1;

        int sum = targetNumber + reverseNumber;
        printf("%d\n", sum);
        fflush(stdout);

        input = scanf("%d", &targetNumber);
    }

    // Trigger error if exited loop without EOF signal
    if (input != -1) {
        fprintf(stderr, "Error: not a number\n");
        return 1;
    }

    return 0;
}