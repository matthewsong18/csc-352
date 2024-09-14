/*
 * File: isFactorial.c
 * Author: Matthew W. Song
 * Purpose: The program reads in a sequence of 0 or more positive integer values from stdin until no more can be read
 * in; and for each value N it reads, prints out "N = b!" if there is an integer b so that N = b!, and "N not
 * factorial" if no such b
*/

#include <stdio.h>

/*
 * Determines whether the target number is a factorial or not. Will print the result to
 * the standard out buffer, or stdout.
 *
 * Returns: 1 if an unexpected error occurs. 0 otherwise.
 */
int isFactorial(int targetNumber) {
    int factorial = 1;
    int factorialIndex = 1;

    while (factorial <= targetNumber) {
        if (factorial == targetNumber) {
            printf("%d = %d!\n", targetNumber, factorialIndex);
            fflush(stdout);
            return 0;
        }

        factorialIndex += 1;
        factorial = factorial * factorialIndex;
    }

    if (factorial > targetNumber) {
        printf("%d not factorial\n", targetNumber);
        fflush(stdout);
        return 0;
    }

    return 1;
}

/*
 * Reads integers from the standard input buffer, or stdin, until it hits an EOF signal or receives a
 * value that is not an integer. These integers will be passed into the isFactorial function every time the Enter key
 * is pressed.
 *
 * Returns: 1 if an error occurred. 0 otherwise.
 */
int main(void) {
    int targetNumber;

    int input = scanf("%d", &targetNumber);

    // Greater than 0 should end on non-integers and EOF
    while (input > 0) {
        if (targetNumber < 0) {
            fprintf(stderr, "Error: Not a positive integer\n");
        }

        if (isFactorial(targetNumber) == 1) {
            return 1;
        }

        input = scanf("%d", &targetNumber);
    }

    // Trigger error if exited loop without EOF signal
    if (input != -1) {
        fprintf(stderr, "Error: Input is not a number\n");
        return 1;
    }

    return 0;
}