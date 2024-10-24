// TODO: Doc

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  // pointer reserved for allocating space for anagram with malloc
  char *anagram;
  struct Node *string_match;
  struct Node *next;
} Node;

typedef struct CountNode {
  char letter;
  int count;
  struct CountNode *next;
} CountNode;

Node *build_linked_list(int *errorStatus);

void print_anagram_list(Node *head);

void free_anagram_list(Node *head);

int read_single_input(char **outputString);

char *to_lowercase(char *buffer);

Node *create_node(char *anagram);

int is_match(char *anagram, char *string);

void append_node(Node *head, Node *newNode);

void print_match_list(Node *head);

void free_match_list(Node *head);

CountNode *create_count_node(char letter);

CountNode *count_letters(char *string);

CountNode *selection_sort(CountNode *head);

// TODO: Doc
int main(void) {
  int errorStatus = 0;

  Node *head = build_linked_list(&errorStatus);

  print_anagram_list(head);

  free_anagram_list(head);

  return errorStatus;
}

/**
 * Function: build_linked_list
 *
 * Builds the anagram linked list by reading from the stdin input stream
 *
 * @param errorStatus A pointer to the errorStatus location to save whether an
 * error occured while building the linked list
 *
 * @return Returns the head of the anagram linked list
 */
Node *build_linked_list(int *errorStatus) {
  // Loop through the input stream one string at a time until the input stream
  // hits EOF
  char *buffer =
      malloc(65 * sizeof(char)); // Used as a buffer for read_single_input

  if (buffer == NULL) {
    fprintf(stderr, "Error: Out of memory");
    exit(1);
  }

  int readStatus;

  int firstRead = 1; // Tracks if the first read so head get's initialzied
  Node *head =
      NULL; // The head pointer used to get to the head of the linked list
  Node *current = NULL; // The pointer used to keep track of the current node

  while ((readStatus = read_single_input(&buffer)) == 0) {

    // Check if first read
    if (firstRead) {
      head = create_node(buffer);
      current = head;
      firstRead = 0;
      continue;
    }

    // Check if the `string` matches an anagram in the `anagram` linked list.
    current = head;
    int duplicate = 0;
    int match = 0;

    while (current) {

      if (strcmp(current->anagram, buffer) == 0) {
        duplicate = 1;
        break;
      }

      // Skip to next anagram if no match
      if (!is_match(current->anagram, buffer)) {
        current = current->next;
        continue;
      }

      match = 1;

      // String matches anagram so check if string already exists in the linked
      // list
      Node *current_match = current->string_match;

      // Checking for duplicates
      while (current_match) {

        if (strcmp(current_match->anagram, buffer) == 0) {
          append_node(current_match, create_node(buffer));
          duplicate = 1;
          break;
        }

        current_match = current_match->next;
      }

      if (duplicate)
        break;

      // No duplicate found so create a new node for the `string` at the tail of
      // the `string_match` linked list.

      if (current->string_match == NULL) {
        current->string_match = create_node(buffer);
        break;
      }

      append_node(current->string_match, create_node(buffer));
      break;
    }

    if (duplicate)
      continue;

    if (match)
      continue;

    // If there is no anagram matching the `string`, add a new anagram node at
    // the tail of the `anagram` linked list.
    append_node(head, create_node(buffer));
  }
  // Check status
  if (readStatus == 1)
    *errorStatus = 1;

  return head;
}

/**
 * Function: print_anagram_list
 *
 * Prints the data of the anagram linked list. Each anagram and it's matching
 * strings will be printed on the same line, a new line for each anagram.
 *
 * Example line:
 *
 *    anagram matching_string1 matching_string2 ... matching_stringNth
 *
 * @param head The head pointer for the anagram linked list
 *
 * @return No return value
 */
void print_anagram_list(Node *head) {
  while (head) {
    printf("%s", head->anagram);
    print_match_list(head->string_match);
    printf("\n");
    fflush(stdout);
    head = head->next;
  }
}

/**
 * Function: free_anagram_list
 *
 * Frees up the anagram linked list recursively
 *
 * @param head The head pointer for the anagram linked list
 *
 * @return No return value
 */
void free_anagram_list(Node *head) {
  // Base case
  if (head == NULL)
    return;

  // Free head node data
  free(head->anagram);
  free_match_list(head->string_match);

  // Free following Node
  free_anagram_list(head->next);

  // Free head
  free(head);
}

/**
 * Function: read_single_input
 *
 * Reads in a single string from the stdin input stream.
 *
 * @param outputString The location where the read string will be saved
 *
 * @return Returns 0 for success, 1 for error, and -1 for EOF
 */
int read_single_input(char **outputString) {
  // Grab one `string` from the input stream.
  int readStatus;

  readStatus = scanf("%64s", *outputString);

  // alphabetic character strings only
  if (readStatus == EOF)
    return -1;

  if (!isalpha(**outputString))
    return 1;

  return 0;
}

/**
 * TODO
 */
char *to_lowercase(char *buffer) {
  int len = strlen(buffer);
  char *new_string = malloc((len + 1) * sizeof(char));

  if (new_string == NULL) {
    fprintf(stderr, "Error: Out of Memory");
    exit(1);
  }

  for (int i = 0; i < len; ++i) {
    new_string[i] = tolower(buffer[i]);
  }

  new_string[len] = '\0';

  return new_string;
}

/**
 * Function: create_node
 *
 * Creates a new node, storing a string anagram
 *
 * @param anagram The string anagram that is being stored in the Node
 *
 * @return The pointer towards the new node
 */
Node *create_node(char *anagram) {
  Node *new_node = malloc(sizeof(Node));

  if (new_node == NULL) {
    fprintf(stderr, "Error: Out of memory");
    exit(1);
  }

  new_node->anagram = strdup(anagram); // Create a new copy of the string

  if (new_node->anagram == NULL) {
    fprintf(stderr, "Error: Out of memory for string");
    exit(1);
  }

  new_node->string_match = NULL;
  new_node->next = NULL;

  return new_node;
}

/**
 * Function: is_match
 *
 * Compares anagram and string to see if string matches to the anagram
 *
 * @param anagram The anagram being compared against
 *
 * @param string The string trying to be matched to the anagram
 *
 * @return Returns 1 string matches the anagram. 0 if there's no match.
 */
int is_match(char *anagram, char *string) {
  if (strlen(anagram) != strlen(string))
    return 0;

  // Make lowercase
  char *anagram_lower = to_lowercase(anagram);
  char *string_lower = to_lowercase(string);

  // Count letters
  CountNode *anagram_head = count_letters(anagram_lower);
  CountNode *string_head = count_letters(string_lower);

  // Sort Linked Lists
  anagram_head = selection_sort(anagram_head);
  string_head = selection_sort(string_head);

  // Compare Count Linked Lists
  CountNode *temp1 = anagram_head;
  CountNode *temp2 = string_head;

  while (temp1 && temp2) {
    if (temp1->letter != temp2->letter || temp1->count != temp2->count) {
      return 0; // If any letter count differs, it's not a match
    }
    temp1 = temp1->next;
    temp2 = temp2->next;
  }

  if (temp1 != NULL || temp2 != NULL) {
    return 0; // If one list is longer than the other, it's not a match
  }

  return 1;
}

/**
 * Function: append_node
 *
 * Appends a new node at the end of the linked list head points towards
 *
 * @param head The head pointer for the target linked list being appended to
 *
 * @param newNode The pointer towards the newNode being appended
 *
 * @return Returns the head pointer
 */
void append_node(Node *head, Node *newNode) {
  Node *temp_ptr = head;

  while (temp_ptr->next != NULL) {
    temp_ptr = temp_ptr->next;
  }

  temp_ptr->next = newNode;
}

/**
 * Function: print_match_list
 *
 * Prints the matching strings for an anagram.
 *
 * @param head The head pointer for the matching strings linked list
 *
 * @return No return value
 */
void print_match_list(Node *head) {
  while (head) {
    printf(" %s", head->anagram);
    head = head->next;
  }
}

/**
 * Function: free_match_list
 *
 * Frees up the matching string linked list recursively.
 *
 * @param head The head pointer for the anagram linked list
 *
 * @return No return value
 */
void free_match_list(Node *head) {
  // Base case
  if (head == NULL)
    return;

  free_match_list(head->next);
  free(head);
}

/**
 * TODO
 */
CountNode *create_count_node(char letter) {
  CountNode *new_count_node = malloc(sizeof(CountNode));

  if (new_count_node == NULL) {
    fprintf(stderr, "Error: Out of memory");
    exit(1);
  }

  new_count_node->letter = letter;
  new_count_node->count = 1;
  new_count_node->next = NULL;

  return new_count_node;
}

/**
 * TODO
 */
CountNode *count_letters(char *string) {
  CountNode *head;
  CountNode *current;

  for (int i = 0; i < strlen(string); ++i) {

    // Empty Linked List
    if (i == 0) {
      head = create_count_node(string[i]);
      current = head;
      continue;
    }

    // Update count or add letter count to linked list
    while (current) {
      if (current->letter == string[i]) {
        current->count++;
        break;
      }

      if (current->next == NULL) {
        current->next = create_count_node(string[i]);
        break;
      }

      current = current->next;
    }
  }

  return head;
}

/**
 * TODO
 */
CountNode *selection_sort(CountNode *head) {
  CountNode *min;
  CountNode *current = head;

  while (current) {
    min = current;
    CountNode *temp = current;

    // Find letter closest to a
    while (temp) {
      if (temp->letter < min->letter)
        min = temp;
      temp = temp->next;
    }

    if (min == current) {
      current = current->next;
      continue;
    }

    // Swap data
    char hold_letter = current->letter;
    int hold_count = current->count;
    current->letter = min->letter;
    current->count = min->count;
    min->letter = hold_letter;
    min->count = hold_count;

    current = current->next;
  }

  return head;
}
