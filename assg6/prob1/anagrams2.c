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

void to_lowercase(char *buffer);

Node *create_node(char *anagram);

int is_match(char *anagram, char *string);

Node *append_node(Node *head, Node *newNode);

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
  int readStatus;

  int firstRead = 1; // Tracks if the first read so head get's initialzied
  Node *head;    // The head pointer used to get to the head of the linked list
  Node *current; // The pointer used to keep track of the current node

  while ((readStatus = read_single_input(&buffer)) > 0) {

    // Check status
    if (readStatus == -1)
      break;
    if (readStatus == 1)
      *errorStatus = 1;

    // Change to lowercase
    to_lowercase(buffer);

    // Check if first read
    if (firstRead) {
      head = create_node(buffer);
      current = head;
      continue;
    }

    // Check if the `string` matches an anagram in the `anagram` linked list.
    int duplicate = 0;

    while (current) {

      Node *current_match = current->string_match;

      // Checking for duplicates
      while (current_match) {

        if (current_match->anagram == buffer) {
          duplicate = 1;
          break;
        }
      }

      if (duplicate)
        break;

      // If it matches, create a new node for the `string` at the tail of the
      // `string_match` linked list.
      if (is_match(current->anagram, buffer)) {
        append_node(current->string_match, create_node(buffer));
      }
    }

    if (duplicate)
      continue;

    // If there is no anagram matching the `string`, add a new anagram node at
    // the tail of the `anagram` linked list.
    append_node(head, create_node(buffer));
  }

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

  readStatus = scanf("%s", *outputString);

  // alphabetic character strings only
  if (readStatus == -1)
    return -1;

  if (readStatus == 0)
    return -1;

  if (!isalpha(**outputString))
    return 1;

  return 0;
}

/**
 * TODO
 */
void to_lowercase(char *buffer) {
  for (int i = 0; i < strlen(buffer); ++i) {
    buffer[i] = tolower(buffer[i]);
  }
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

  new_node->anagram = anagram;
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

  // Count letters
  CountNode *anagram_head = count_letters(anagram);
  CountNode *string_head = count_letters(string);

  // Sort Linked Lists
  anagram_head = selection_sort(anagram_head);
  string_head = selection_sort(string_head);

  // Compare Count Linked Lists
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
Node *append_node(Node *head, Node *newNode) {
  Node *temp_ptr = head;

  while (temp_ptr->next != NULL) {
    temp_ptr = temp_ptr->next;
  }

  temp_ptr->next = newNode;

  return head;
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
    printf("%s", head->anagram);
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
