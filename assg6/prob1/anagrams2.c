// TODO: Doc

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  // pointer reserved for allocating space for anagram with malloc
  char *anagram;
  struct Node *string_match;
  struct Node *next;
} Node;

Node *build_linked_list(int *errorStatus);

void print_anagram_list(Node *head);

void free_anagram_list(Node *head);

int read_single_input(char **outputString);

Node *create_node(char *anagram);

Node *append_node(Node *head, Node *newNode);

void print_match_list(Node *head);

void free_match_list(Node *head);

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
  char **buffer;
  int readStatus;

  while ((readStatus = read_single_input(buffer)) > 0) {
    // TODO: Do stuff

    // Check status

    // Check if the `string` matches an anagram in the `anagram` linked list.

    // If it matches, create a new node for the `string` at the tail of the
    // `string_match` linked list.

    // If there is no anagram matching the `string`, add a new anagram node at
    // the tail of the `anagram` linked list.
  }

  return NULL;
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
  // TODO
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
  // TODO
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
 * Function: create_node
 *
 * Creates a new node, storing a string anagram
 *
 * @param anagram The string anagram that is being stored in the Node
 *
 * @return The pointer towards the new node
 */
Node *create_node(char *anagram) {
  Node *newNode = malloc(sizeof(Node));

  if (newNode == NULL) {
    fprintf(stderr, "Error: Out of memory");
    exit(1);
  }

  newNode->anagram = anagram;
  newNode->string_match = NULL;
  newNode->next = NULL;

  return newNode;
}

/**
 * Function: read_single_input
 *
 * Reads in a single string from the stdin input stream.
 *
 * @param outputString The location where the read string will be saved
 *
 * @return Returns 1 for an error and 0 for success
 */
int read_single_input(char **outputString) {
  // TODO
  // Grab one `string` from the input stream.

  // 64 length string buffer
  // alphabetic character strings only

  // Save string in outputString

  return 0;
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
