/*
 * File: getPrime.c
 * Author: Matthew W. Song
 * Purpose: The program reads in a positive integer from stdin and prints out the smallest prime number that is larger
 * than n
*/

#include <stdio.h>
#include <math.h>

/*
 * Finds the smallest prime above the provided number by searching for an integer that can divide the number from 2
 * up until the number's square root (upper bound)
 *
 * Returns: -1 if an error occurs. An integer prime number greater than number otherwise.
 */
int getSmallestPrime(int number) {
    if (number < 0) return -1;

    int primeTestNumber = number + 1;

    double upperBound = sqrt(primeTestNumber);

    if (pow(floor(upperBound), 2) == primeTestNumber) {
        return getSmallestPrime(primeTestNumber);
    }

    for (int i = 2; i < upperBound; i++) {
        if (primeTestNumber % i == 0) return getSmallestPrime(primeTestNumber);
    }

    return primeTestNumber;
}

/*
 * Read in a sequence of positive integers and calculate the smallest prime number greater than each integer.
 *
 * Returns: 1 if an error occurs. 0 otherwise.
 */
int main(void) {
    int targetNumber;

    int input = scanf("%d", &targetNumber);

    // Greater than 0 should end on non-integers and EOF
    if (input > 0) {
        if (targetNumber < 0) {
            fprintf(stderr, "Error: not a positive integer\n");
        }

        int smallestPrime = getSmallestPrime(targetNumber);
        if (smallestPrime == -1) return 1;

        printf("%d\n", smallestPrime);
        fflush(stdout);
        return 0;
    }

    // Trigger error if exited loop without EOF signal
    if (input != -1) {
        fprintf(stderr, "Error: not a number\n");
        return 1;
    }

    return 0;
}