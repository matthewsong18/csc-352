#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedList {
  int number;
  int count;
  struct LinkedList *next;
} LinkedList;

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

      if (current_ptr->number == numberBuffer) {
        current_ptr->count++;
        break;
      }

      else if (current_ptr->next == NULL) {
        current_ptr->next = newList(numberBuffer);
        break;
      }

      else {
        current_ptr = current_ptr->next;
      }
    }
    current_ptr = head;
  }

  if (retval != -1) {
    fprintf(stderr, "Error: Non-integer characters in input\n");
    exit(1);
  }

  // Sort linked list
  current_ptr = head;
  LinkedList *min;
  int length = 0;

  while (current_ptr) {
    ++length;
    current_ptr = current_ptr->next;
  }

  current_ptr = head;

  for (int i = 0; i < length; ++i) {
    min = current_ptr;
    LinkedList *temp = current_ptr;

    while (temp) {
      if (temp->number < min->number)
        min = temp;
      temp = temp->next;
    }

    if (min == current_ptr) {
      current_ptr = current_ptr->next;
      continue;
    }

    int holdNumber = current_ptr->number;
    int holdCount = current_ptr->count;
    current_ptr->number = temp->number;
    current_ptr->count = temp->count;
    temp->number = holdNumber;
    temp->count = holdCount;

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
