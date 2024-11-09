/*
 * File: graph.c
 * Author: Matthew W Song
 * Purpose: Implements a graph data structure, including nodes and edges,
 *          and provides functionality for reading input files, handling user
 *          queries, and calculating the shortest path between nodes.
 */

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error enums used for easy error handling
typedef enum ErrorType {
  ERROR_INPUT_FILE,
  ERROR_CLI_ARGS,
  ERROR_PARSE_LINE,
  ERROR_MALLOC,
  ERROR_SCAN_LINE,
  ERROR_MISSING_NODE,
  ERROR_MISSING_EDGE,
  ERROR_NON_ALPHA,
  ERROR_NEGATIVE,
} ErrorType;

typedef struct Node {
  char *name;
  struct Edge *edgeList;
  struct Node *next;
  int visited;
  int minDist;
} Node;

typedef struct Edge {
  Node *destination;
  struct Edge *next;
  int weight;
} Edge;

typedef struct PQNode {
  Node *node;
  struct PQNode *next;
} PQNode;

const int STATUS_OKAY = 0;
const int STATUS_ERROR = 1;

int getInputFile(int argc, char **argv, char **filename);
void handleError(ErrorType error);
int makeGraph(char *filename, Node **headPtr);
int handleUserQueries(Node *graphPtr);
void freeGraph(Node *graphPtr);
int parseLine(char *line, char **name1, char **name2, int *distance);
int isAlphabetic(const char *name);
Node *createNode(char *name);
void addEdges(Node *node1, Node *node2, int distance);
void addNodes(Node **headPtr, Node *node1, Node *node2);
int parseQuery(char *line, char **name1, char **name2);
int shortestPathAlgorithm(Node *graphPtr, Node *node1, Node *node2);
int calculateDistance(Node *node);
void insertNode(PQNode **head, Node *node);
Node *extractMin(PQNode **head);

int main(int argc, char *argv[]) {
  int errorStatus = STATUS_OKAY;

  char *filename = NULL;

  const int inputFileStatus = getInputFile(argc, argv, &filename);

  if (inputFileStatus == STATUS_ERROR) {
    errorStatus = STATUS_ERROR;
    return errorStatus; // Early return if error
  }

  Node *graphPtr = NULL;
  const int graphStatus = makeGraph(filename, &graphPtr);

  if (graphPtr == NULL) {
    errorStatus = STATUS_ERROR;
  }

  if (graphStatus == STATUS_ERROR) {
    errorStatus = STATUS_ERROR;
  }

  const int userQueryStatus = handleUserQueries(graphPtr);

  if (userQueryStatus == STATUS_ERROR) {
    errorStatus = STATUS_ERROR;
  }

  fflush(stdout);

  freeGraph(graphPtr);

  return errorStatus;
}

/*
 * getInputFile(argc, argv, filename) -- Extracts the input filename from
 * command-line arguments.
 * Arguments:
 *   argc - number of command-line arguments
 *   argv - array of command-line arguments
 *   filename - pointer to store the extracted filename
 * Returns:
 *   STATUS_OKAY on success, STATUS_ERROR if there's an error with arguments
 *   or memory allocation.
 */

int getInputFile(int argc, char **argv, char **filename) {
  if (argc != 2) {
    handleError(ERROR_CLI_ARGS);
    return STATUS_ERROR;
  }

  *filename = strdup(argv[1]);
  if (*filename == NULL) {
    handleError(ERROR_MALLOC);
    return STATUS_ERROR;
  }

  return STATUS_OKAY;
}

/*
 * handleError(error) -- Outputs an appropriate error message based on the
 * given error type.
 * Arguments:
 *   error - an ErrorType value indicating the type of error
 * Returns:
 *   None (void)
 */
void handleError(ErrorType error) {
  switch (error) {
  case ERROR_INPUT_FILE:
    fprintf(stderr, "ERROR: Failed to read Input File\n");
    break;
  case ERROR_CLI_ARGS:
    fprintf(stderr, "ERROR: Incorrect number of CLI arguments\n");
    break;
  case ERROR_PARSE_LINE:
    fprintf(stderr, "ERROR: Failed to parse line\n");
    break;
  case ERROR_MALLOC:
    fprintf(stderr, "ERROR: Ran out of memory\n");
    break;
  case ERROR_SCAN_LINE:
    fprintf(stderr, "ERROR: Failed to scan line\n");
    break;
  case ERROR_MISSING_NODE:
    fprintf(stderr, "ERROR: Could not find node\n");
    break;
  case ERROR_MISSING_EDGE:
    fprintf(stderr, "ERROR: Could not find edge\n");
    break;
  case ERROR_NON_ALPHA:
    fprintf(stderr, "ERROR: Input name is not alphabetical\n");
    break;
  case ERROR_NEGATIVE:
    fprintf(stderr, "ERROR: Negatives are not allowed\n");
    break;
  }
}

/*
 * makeGraph(filename, headPtr) -- Reads the graph data from a file and
 * creates the corresponding graph structure.
 * Arguments:
 *   filename - name of the input file containing graph data
 *   headPtr - pointer to the head of the Node linked list
 * Returns:
 *   STATUS_OKAY on success, STATUS_ERROR on failure (e.g., file read error).
 */
int makeGraph(char *filename, Node **headPtr) {
  int errorStatus = STATUS_OKAY;
  FILE *inputFile = fopen(filename, "r");

  if (inputFile == NULL) {
    handleError(ERROR_INPUT_FILE);
    errorStatus = STATUS_ERROR;
  }

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  // Loop through inputFile
  while ((linelen = getline(&line, &linecap, inputFile)) > 0) {
    char *name1 = NULL;
    char *name2 = NULL;
    int distance = 0;

    // Read line
    int parseStatus = parseLine(line, &name1, &name2, &distance);

    if (parseStatus == STATUS_ERROR) {
      handleError(ERROR_PARSE_LINE);
      errorStatus = STATUS_ERROR;
      free(line);
      line = NULL;
      continue;
    }

    free(line);
    line = NULL;

    Node *node1 = NULL;
    Node *node2 = NULL;
    Node *currentPtr = NULL;

    // Make sure headPtr is set
    if (*headPtr == NULL) {
      node1 = createNode(name1);
      node2 = createNode(name2);
      addEdges(node1, node2, distance);
      addNodes(headPtr, node1, node2);
      free(name1);
      free(name2);
      continue;
    }

    currentPtr = *headPtr;

    // Loop through graph to check if node exists
    while (currentPtr != NULL) {
      // Exit loop if nodes are set
      if (node1 != NULL && node2 != NULL)
        break;

      if (strcmp(currentPtr->name, name1) == 0)
        node1 = currentPtr;
      if (strcmp(currentPtr->name, name2) == 0)
        node2 = currentPtr;

      currentPtr = currentPtr->next;
    }

    if (node1 == NULL)
      node1 = createNode(name1);
    if (node2 == NULL)
      node2 = createNode(name2);

    // Add to graph
    addEdges(node1, node2, distance);
    addNodes(headPtr, node1, node2);

    free(name1);
    free(name2);
  }

  free(filename);

  return errorStatus;
}

/*
 * freeGraph(graphPtr) -- Frees memory allocated for the entire graph
 * structure, including nodes and edges.
 * Arguments:
 *   graphPtr - pointer to the head of the graph Node list
 * Returns:
 *   None (void)
 */
void freeGraph(Node *graphPtr) {
  Node *currentNode = graphPtr;
  while (currentNode != NULL) {
    Node *nextNode = currentNode->next;

    // Free edges
    Edge *currentEdge = currentNode->edgeList;
    while (currentEdge != NULL) {
      Edge *nextEdge = currentEdge->next;
      free(currentEdge);
      currentEdge = nextEdge;
    }

    free(currentNode->name);
    free(currentNode);

    currentNode = nextNode;
  }
}

/*
 * parseLine(line, name1, name2, distance) -- Parses a line from the input
 * file to extract node names and edge distance.
 * Arguments:
 *   line - input line to parse
 *   name1 - pointer to store the first node name
 *   name2 - pointer to store the second node name
 *   distance - pointer to store the edge distance
 * Returns:
 *   STATUS_OKAY on successful parsing, STATUS_ERROR if parsing fails.
 */
int parseLine(char *line, char **name1, char **name2, int *distance) {
  int errorStatus = STATUS_OKAY;

  *name1 = malloc(65 * sizeof(char));
  if (*name1 == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }

  *name2 = malloc(65 * sizeof(char));
  if (*name2 == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }

  int scanStatus = sscanf(line, "%64s %64s %d", *name1, *name2, distance);

  if (scanStatus != 3) {
    handleError(ERROR_SCAN_LINE);
    errorStatus = STATUS_ERROR;
    free(*name1);
    free(*name2);
    *name1 = NULL;
    *name2 = NULL;
    return errorStatus;
  }

  if (*name1 == NULL || *name2 == NULL) {
    handleError(ERROR_SCAN_LINE);
    errorStatus = STATUS_ERROR;
    return errorStatus;
  }

  if (!isAlphabetic(*name1)) {
    handleError(ERROR_NON_ALPHA);
    errorStatus = STATUS_ERROR;
    free(*name1);
    free(*name2);
    *name1 = NULL;
    *name2 = NULL;
    return errorStatus;
  }

  if (!isAlphabetic(*name2)) {
    handleError(ERROR_NON_ALPHA);
    errorStatus = STATUS_ERROR;
    free(*name1);
    free(*name2);
    *name1 = NULL;
    *name2 = NULL;
    return errorStatus;
  }

  if (*distance < 0) {
    handleError(ERROR_NEGATIVE);
    errorStatus = STATUS_ERROR;
    free(*name1);
    free(*name2);
    *name1 = NULL;
    *name2 = NULL;
  }

  return errorStatus;
}

/*
 * isAlphabetic(name) -- Checks if a given string contains only alphabetic
 * characters.
 * Arguments:
 *   name - string to check
 * Returns:
 *   1 if the string is alphabetic, 0 otherwise.
 */
int isAlphabetic(const char *name) {
  int isAlpha = 1;

  int length = strlen(name);
  for (int i = 0; i < length; ++i) {
    if (!isalpha(name[i]))
      isAlpha = 0;
  }

  return isAlpha;
}

/*
 * createNode(name) -- Creates a new Node with the given name.
 * Arguments:
 *   name - name for the new node
 * Returns:
 *   A pointer to the newly created Node, or exits if memory allocation fails.
 */
Node *createNode(char *name) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }

  node->name = strdup(name);
  if (node->name == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }
  node->edgeList = NULL;
  node->next = NULL;
  node->visited = 0;
  node->minDist = INT_MAX;

  return node;
}

Edge *createEdge(Node *target, int distance) {
  Edge *edge = malloc(sizeof(Edge));
  if (edge == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }
  edge->destination = target;
  edge->next = NULL;
  edge->weight = distance;

  return edge;
}

/*
 * addEdges(node1, node2, distance) -- Adds an edge between two nodes with
 * a specified distance.
 * Arguments:
 *   node1 - pointer to the first Node
 *   node2 - pointer to the second Node
 *   distance - weight of the edge between node1 and node2
 * Returns:
 *   None (void)
 */
void addEdges(Node *node1, Node *node2, int distance) {
  if (node1->edgeList == NULL) {
    node1->edgeList = createEdge(node2, distance);
  } else {
    Edge *current_edge = node1->edgeList;
    Edge *last_ptr = current_edge;
    int isFound = 0;

    while (current_edge != NULL) {
      last_ptr = current_edge;

      if (current_edge->destination == node2) {
        isFound = 1;
        break;
      }

      current_edge = current_edge->next;
    }

    if (!isFound) {
      last_ptr->next = createEdge(node2, distance);
    }
  }

  if (node2->edgeList == NULL) {
    node2->edgeList = createEdge(node1, distance);
  } else {
    Edge *current_edge = node2->edgeList;
    Edge *last_ptr = current_edge;
    int isFound = 0;

    while (current_edge != NULL) {
      last_ptr = current_edge;

      if (current_edge->destination == node2) {
        isFound = 1;
        break;
      }

      current_edge = current_edge->next;
    }

    if (!isFound) {
      last_ptr->next = createEdge(node1, distance);
    }
  }
}

/*
 * addNodes(headPtr, node1, node2) -- Adds nodes to the graph if they don't
 * already exist.
 * Arguments:
 *   headPtr - pointer to the head of the Node linked list
 *   node1 - pointer to the first Node to add
 *   node2 - pointer to the second Node to add
 * Returns:
 *   None (void)
 */
void addNodes(Node **headPtr, Node *node1, Node *node2) {
  int isNode1Found = 0;
  int isNode2Found = 0;

  if (*headPtr == NULL) {
    *headPtr = node1;
    node1->next = node2;
    return;
  }

  Node *current = *headPtr;
  Node *last = current;

  while (current != NULL) {
    last = current;

    if (isNode1Found && isNode2Found)
      break;

    if (current == node1)
      isNode1Found = 1;

    if (current == node2)
      isNode2Found = 1;

    current = current->next;
  }

  if (!isNode1Found) {
    last->next = node1;
    last = last->next;
  }

  if (!isNode2Found) {
    last->next = node2;
  }
}

/*
 * handleUserQueries(graphPtr) -- Processes user queries to calculate
 * shortest paths in the graph.
 * Arguments:
 *   graphPtr - pointer to the head of the graph Node list
 * Returns:
 *   STATUS_OKAY on successful handling of queries, STATUS_ERROR on failure.
 */
int handleUserQueries(Node *graphPtr) {
  int errorStatus = STATUS_OKAY;

  char *line = NULL;
  size_t linecap = 0;
  ssize_t linelen;

  while ((linelen = getline(&line, &linecap, stdin) > 0)) {
    char *name1 = NULL;
    char *name2 = NULL;

    // Get names
    int parseStatus = parseQuery(line, &name1, &name2);

    if (parseStatus == STATUS_ERROR) {
      errorStatus = STATUS_ERROR;
      free(line);
      line = NULL;
      continue;
    }

    free(line);
    line = NULL;

    Node *current = graphPtr;
    Node *node1 = NULL;
    Node *node2 = NULL;

    // Find nodes
    while (current != NULL) {
      if (node1 != NULL && node2 != NULL)
        break;

      if (strcmp(current->name, name1) == 0)
        node1 = current;
      if (strcmp(current->name, name2) == 0)
        node2 = current;

      current = current->next;
    }

    free(name1);
    free(name2);

    // Check if failed to find node
    if (node1 == NULL || node2 == NULL) {
      handleError(ERROR_MISSING_NODE);
      errorStatus = STATUS_ERROR;
      continue;
    }

    // Find shortest path
    int algorithmStatus = shortestPathAlgorithm(graphPtr, node1, node2);

    if (algorithmStatus == STATUS_ERROR) {
      errorStatus = STATUS_ERROR;
      continue;
    }
  }

  return errorStatus;
}

/*
 * parseQuery(line, name1, name2) -- Parses a user query to extract two node
 * names.
 * Arguments:
 *   line - query line to parse
 *   name1 - pointer to store the first node name from the query
 *   name2 - pointer to store the second node name from the query
 * Returns:
 *   STATUS_OKAY on success, STATUS_ERROR if parsing fails.
 */
int parseQuery(char *line, char **name1, char **name2) {
  int errorStatus = STATUS_OKAY;

  *name1 = malloc(65 * sizeof(char));
  if (*name1 == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }

  *name2 = malloc(65 * sizeof(char));
  if (*name2 == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }

  char extra[2] = "";

  int parseStatus = sscanf(line, "%64s %64s %1s", *name1, *name2, extra);

  if (parseStatus != 2) {
    handleError(ERROR_PARSE_LINE);
    errorStatus = STATUS_ERROR;
    free(*name1);
    free(*name2);
    *name1 = NULL;
    *name2 = NULL;
  }

  return errorStatus;
}

/*
 * shortestPathAlgorithm(graphPtr, node1, node2) -- Calculates the shortest
 * path between two nodes using Dijkstra's algorithm.
 * Arguments:
 *   graphPtr - pointer to the head of the graph Node list
 *   node1 - pointer to the start Node for the path
 *   node2 - pointer to the destination Node for the path
 * Returns:
 *   STATUS_OKAY if path is found, STATUS_ERROR if no path exists.
 */
int shortestPathAlgorithm(Node *graphPtr, Node *node1, Node *node2) {
  int errorStatus = STATUS_OKAY;

  // Initialize all nodes
  Node *current = graphPtr;
  while (current != NULL) {
    current->minDist = INT_MAX;
    current->visited = 0;
    current = current->next;
  }
  node1->minDist = 0;

  // Initialize the priority queue
  PQNode *priorityQueue = NULL;

  // Insert the start node into the priority queue
  insertNode(&priorityQueue, node1);

  while (priorityQueue != NULL) {
    // Extract the node with the smallest minDist
    Node *currentNode = extractMin(&priorityQueue);

    if (currentNode->visited)
      continue;

    currentNode->visited = 1;

    // For each neighbor of currentNode
    Edge *edge = currentNode->edgeList;
    while (edge != NULL) {
      Node *neighbor = edge->destination;

      // Calculate the new tentative distance
      int newDist = currentNode->minDist + edge->weight;

      if (newDist < neighbor->minDist) {
        neighbor->minDist = newDist;
        // Insert neighbor into the priority queue
        insertNode(&priorityQueue, neighbor);
      }

      edge = edge->next;
    }
  }

  if (node2->minDist == INT_MAX) {
    // No path found
    handleError(ERROR_MISSING_EDGE);
    errorStatus = STATUS_ERROR;
  } else {
    // The shortest distance is in node2->minDist
    printf("%d\n", node2->minDist);
  }

  return errorStatus;
}

/*
 * insertNode(head, node) -- Inserts a node into a priority queue based on
 * its minDist value.
 * Arguments:
 *   head - pointer to the head of the priority queue
 *   node - pointer to the Node to insert
 * Returns:
 *   None (void)
 */
void insertNode(PQNode **head, Node *node) {
  PQNode *newPQNode = malloc(sizeof(PQNode));
  if (newPQNode == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }
  newPQNode->node = node;
  newPQNode->next = NULL;

  // If the queue is empty or the new node has smaller minDist than the head
  if (*head == NULL || node->minDist < (*head)->node->minDist) {
    newPQNode->next = *head;
    *head = newPQNode;
    return;
  }

  // Find the correct position to insert
  PQNode *current = *head;
  while (current->next != NULL &&
         current->next->node->minDist <= node->minDist) {
    current = current->next;
  }
  newPQNode->next = current->next;
  current->next = newPQNode;
}

/*
 * extractMin(head) -- Extracts and removes the Node with the smallest minDist
 * from the priority queue.
 * Arguments:
 *   head - pointer to the head of the priority queue
 * Returns:
 *   Pointer to the Node with the smallest minDist, or NULL if the queue is
 *   empty.
 */
Node *extractMin(PQNode **head) {
  if (*head == NULL) {
    return NULL;
  }
  PQNode *minPQNode = *head;
  Node *minNode = minPQNode->node;
  *head = minPQNode->next;
  free(minPQNode);
  return minNode;
}
