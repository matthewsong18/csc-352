#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum ErrorType {
  ERROR_INPUT_FILE,
  ERROR_CLI_ARGS,
  ERROR_PARSE_LINE,
  ERROR_MALLOC,
  ERROR_SCAN_LINE,
  ERROR_MISSING_NODE,
  ERROR_MISSING_EDGE,
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

const int STATUS_OKAY = 0;
const int STATUS_ERROR = 1;

int getInputFile(int argc, char **argv, char **filename);
void handleError(ErrorType error);
int makeGraph(char *filename, Node **headPtr);
int handleUserQueries(Node *graphPtr);
void freeGraph(Node *graphPtr);
int parseLine(const char *line, char **name1, char **name2, int *distance);
int isAlphabetic(const char *name);
Node *createNode(char *name);
void addEdges(Node *node1, Node *node2, int distance);
void addNodes(Node **headPtr, Node *node1, Node *node2);
int parseQuery(char *line, char **name1, char **name2);
int shortestPathAlgorithm(Node *graphPtr, Node *node1, Node *node2);
int calculateDistance(Node *node);

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

// Get inputFile filename
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
  }
}

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

    free(line);
    line = NULL;

    if (parseStatus == STATUS_ERROR) {
      handleError(ERROR_PARSE_LINE);
      errorStatus = STATUS_ERROR;
    }

    Node *node1 = NULL;
    Node *node2 = NULL;
    Node *currentPtr = NULL;

    // Make sure headPtr is set
    if (*headPtr == NULL) {
      node1 = createNode(name1);
      node2 = createNode(name2);
      addEdges(node1, node2, distance);
      addNodes(headPtr, node1, node2);
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

int parseLine(const char *line, char **name1, char **name2, int *distance) {
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
  }

  if (!isAlphabetic(*name1)) {
    free(*name1);
    *name1 = NULL;
  }

  if (!isAlphabetic(*name2)) {
    free(*name2);
    *name2 = NULL;
  }

  if (*distance < 0) {
    *distance = 0;
  }

  return errorStatus;
}

int isAlphabetic(const char *name) {
  int isAlpha = 1;

  int length = strlen(name);
  for (int i = 0; i < length; ++i) {
    if (!isalpha(name[i]))
      isAlpha = 0;
  }

  return isAlpha;
}

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

// Read in all user queries
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

    free(line);
    line = NULL;

    if (parseStatus == STATUS_ERROR) {
      errorStatus = STATUS_ERROR;
    }

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

    if (algorithmStatus == STATUS_ERROR)
      errorStatus = STATUS_ERROR;

    printf("%d\n", node2->minDist);
  }

  return errorStatus;
}

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

  int parseStatus = sscanf(line, "%64s %64s", *name1, *name2);

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

// Calculate the shortest path using Djisktra's algorithm (Assuming there
// are no negative paths)
int shortestPathAlgorithm(Node *graphPtr, Node *node1, Node *node2) {
  int errorStatus = STATUS_OKAY;
  Node *current = graphPtr;

  // Initialization
  while (current != NULL) {
    current->minDist = INT_MAX;
    current->visited = 0;

    current = current->next;
  }
  node1->minDist = 0;

  int calculationStatus = calculateDistance(node1);
  if (calculationStatus == STATUS_ERROR) {
    errorStatus = STATUS_ERROR;
  }

  return errorStatus;
}

int calculateDistance(Node *node) {
  if (node->visited)
    return STATUS_OKAY;

  node->visited = 1;

  if (node->edgeList == NULL) {
    handleError(ERROR_MISSING_EDGE);
    return STATUS_ERROR;
  }

  Edge *currentEdge = node->edgeList;
  while (currentEdge != NULL) {
    int distance = node->minDist + currentEdge->weight;

    if (node->minDist == INT_MAX)
      distance = currentEdge->weight;

    if (distance < currentEdge->destination->minDist)
      currentEdge->destination->minDist = distance;

    calculateDistance(currentEdge->destination);

    currentEdge = currentEdge->next;
  }

  return STATUS_OKAY;
}
