/*
 * File: count2.c
 * Author: Matthew W Song
 * Purpose: A program that takes in as many numbers as needed and adds them to a
 * linked list. The linked list holds the number, how many times the number
 * appeared, and the next linked list. If no linked list contains the number
 * read in, a new list is added at the end of the list with a count of 1.
 */

#include <stdio.h>
#include <stdlib.h>

// Linked List containing number and it's count
typedef struct LinkedList {
  int number;
  int count;
  struct LinkedList *next;
} LinkedList;

// A function to allocate enought space for a new Linked List
LinkedList *newList(int number) {
  LinkedList *newList = (LinkedList *)malloc(sizeof(LinkedList));

  if (newList == NULL) {
    fprintf(stderr, "ERROR: Out of memory\n");
    exit(1);
  }

  newList->number = number;
  newList->count = 1;
  newList->next = NULL;

  return newList;
}

int main(void) {
  LinkedList *head;

  int number;
  int status;

  // Read in the first number seperately so we can start the head of the list
  status = scanf("%d", &number);
  if (status == 0) {
    fprintf(stderr, "Error: Non-integer characters in input\n");
    exit(1);
  }

  head = newList(number);

  int numberBuffer;
  LinkedList *current_ptr = head;
  int retval;

  // Read in a sequence of an unknown amount of integers
  // Non-integer values should exit(1)
  // Store input in a linked list
  while ((retval = scanf("%d", &numberBuffer)) > 0) {

    while (current_ptr) {

      // Check if number already has a list
      if (current_ptr->number == numberBuffer) {
        current_ptr->count++;
        break;
      }

      // Makes a new list if one isn't found
      else if (current_ptr->next == NULL) {
        current_ptr->next = newList(numberBuffer);
        break;
      }

      // Goes to the next list
      else {
        current_ptr = current_ptr->next;
      }
    }
    current_ptr = head;
  }

  // A character was input into the stream
  if (retval != -1) {
    fprintf(stderr, "Error: Non-integer characters in input\n");
    exit(1);
  }

  // Sort linked list using selection sort
  current_ptr = head;
  LinkedList *min;
  int length = 0;

  // Counts how many nodes are in the linked list
  while (current_ptr) {
    ++length;
    current_ptr = current_ptr->next;
  }

  current_ptr = head;

  // Selection sort
  for (int i = 0; i < length; ++i) {
    min = current_ptr;
    LinkedList *temp = current_ptr;

    // Compare every node after the current pointer to find the minimum
    while (temp) {
      if (temp->number < min->number)
        min = temp;
      temp = temp->next;
    }

    // If the current pointer is the minimum, go to the next node
    if (min == current_ptr) {
      current_ptr = current_ptr->next;
      continue;
    }

    // Swap the smallest values with the current pointer's values
    int holdNumber = current_ptr->number;
    int holdCount = current_ptr->count;
    current_ptr->number = min->number;
    current_ptr->count = min->count;
    min->number = holdNumber;
    min->count = holdCount;

    current_ptr = current_ptr->next;
  }

  // Count how many times each number appears
  current_ptr = head;
  while (current_ptr) {
    printf("%d %d\n", current_ptr->number, current_ptr->count);
    fflush(stdout);
    current_ptr = current_ptr->next;
  }

  exit(0);
}
