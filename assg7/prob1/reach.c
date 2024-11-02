/**
 * File: reach.c
 * Author: Matthew W. Song
 * Purpose: This program builds a directed graph based on input commands that
 * define nodes and edges. It checks if a path exists between specified nodes
 * using depth-first search, helping to analyze if certain "bad" states are
 * reachable. The program handles errors, reads from a file or standard input,
 * and carefully manages memory for safety.
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node and Graph structures here
typedef struct Node {
  char *name;
  struct Edge *edgeList;
  struct Node *next;
  int visited;
} Node;

typedef struct Edge {
  Node *destination;
  struct Edge *next;
} Edge;

typedef enum {
  ERROR_BAD_OP,
  ERROR_BAD_ARG,
  ERROR_CLI_TOO_MANY_ARGS,
  ERROR_DUPLICATE_NODE,
  ERROR_EDGE_MISSING_NODE,
  ERROR_BAD_INPUT_FILE,
  ERROR_MALLOC
} ErrorType;

char *getInput(FILE *inputFile);
int parseInput(const char *line, char *command, char *arg1, char *arg2);
int checkInput(const char *command, const char *arg1, const char *arg2);
void handleError(ErrorType errorType);
Node *initializeGraph();
void freeGraph(Node *graph);
void freeEdges(Edge *edgeList);
int addNode(Node *graph, const char *nodeName);
int addEdge(Node *graph, const char *vName1, const char *vName2);
int searchPath(Node *graph, const char *startNode, const char *endNode);
int depthFirstSearch(Node *currentNode, Node *targetNode);

int main(int argc, char *argv[]) {
  int errorStatus = 0;

  if (argc > 2) {
    handleError(ERROR_CLI_TOO_MANY_ARGS);
    errorStatus = 1;
  }

  FILE *inputFile = NULL;

  if (argc > 1) {
    inputFile = fopen(argv[1], "r");

    if (inputFile == NULL) {
      printf("Error opening file.\n");
      return 1;
    }
  }

  char *inputLine = NULL;
  char *command = NULL;
  char *arg1 = NULL;
  char *arg2 = NULL;
  Node *head_ptr = initializeGraph();
  int pathExists = 0;

  while (strlen(inputLine = getInput(inputFile)) > 0) {
    command = malloc(4 * sizeof(char));
    arg1 = malloc(65 * sizeof(char));
    arg2 = malloc(65 * sizeof(char));

    if (command)
      strcpy(command, "");
    if (arg1)
      strcpy(arg1, "");
    if (arg2)
      strcpy(arg2, "");

    if (parseInput(inputLine, command, arg1, arg2)) {
      errorStatus = 1;
      free(command);
      free(arg1);
      free(arg2);
      free(inputLine);
      command = NULL;
      arg1 = NULL;
      arg2 = NULL;
      continue;
    }

    Node *ptr;
    Node *start = NULL;
    Node *end = NULL;

    switch (command[1]) {
    case 'n':
      if (addNode(head_ptr, arg1)) {
        errorStatus = 1;
      }
      break;

    case 'e':
      if (addEdge(head_ptr, arg1, arg2)) {
        errorStatus = 1;
      }
      break;

    case 'q':
      ptr = head_ptr;
      while (ptr != NULL) {

        if (strcmp(ptr->name, arg1) == 0) {
          start = ptr;
        }
        if (strcmp(ptr->name, arg2) == 0) {
          end = ptr;
        }

        // Exit early if both found
        if (start != NULL && end != NULL)
          break;

        ptr = ptr->next;
      }

      if (start == NULL || end == NULL) {
        handleError(ERROR_EDGE_MISSING_NODE);
        break;
      }

      pathExists = searchPath(head_ptr, arg1, arg2);

      printf("%d\n",
             pathExists); // pathExists = 1 if there is such a path, 0 otherwise
      break;
    }

    free(command);
    free(arg1);
    free(arg2);
    command = NULL;
    arg1 = NULL;
    arg2 = NULL;
  }

  if (inputFile != NULL)
    fclose(inputFile);

  freeGraph(head_ptr);

  free(inputLine);

  return errorStatus;
}

/**
 * getInput
 * Purpose: Retrieves the next line of input from the specified file or standard
 * input.
 * Arguments:
 *   - inputFile: a pointer to the file to read from; if NULL, reads from stdin.
 * Returns: A dynamically allocated string containing the next line of input, or
 * NULL if EOF. Assumptions: Caller is responsible for freeing the returned
 * string. Notes: Protects against buffer overflow by dynamically resizing as
 * needed.
 */
char *getInput(FILE *inputFile) {
  // Assign memory
  char *line = NULL; // Placeholder pointer for getline to allocate memory and
                     // store the line at
  size_t linecap =
      0; // The capacity of the buffer at line which is allocated by getline

  // Check if there is an inputFile
  if (inputFile != NULL) {

    // Use inputFile if it exists
    getline(&line, &linecap, inputFile);
  } else {

    // Use stdin if inputFile does not exist
    getline(&line, &linecap, stdin);
  }

  return line;
}

/**
 * parseInput
 * Purpose: Parses a line of input into a command and its arguments.
 * Arguments:
 *   - line: a string containing the input line to be parsed.
 *   - command: pointer to a string buffer where the command is stored.
 *   - arg1, arg2: pointers to string buffers where arguments are stored, if
 * present. Returns: 0 if parsing is successful, 1 if an error is encountered.
 * Assumptions: line, command, arg1, and arg2 are non-NULL and of sufficient
 * size.
 */
int parseInput(const char *line, char *command, char *arg1, char *arg2) {
  char extraArg[2] = "";
  sscanf(line, "%3s %64s %64s %1s", command, arg1, arg2, extraArg);

  if (strlen(extraArg) > 0) {
    handleError(ERROR_BAD_ARG);
    return 1;
  }

  return checkInput(command, arg1, arg2);
}

/**
 * checkInput
 * Purpose: Validates the parsed command and arguments.
 * Arguments:
 *   - command: a string containing the command to validate.
 *   - arg1, arg2: strings containing the arguments to validate (if any).
 * Returns: 0 if input is valid, 1 if input is invalid.
 * Assumptions: command is one of "@n", "@e", or "@q"; arg1 and arg2 adhere to
 * requirements.
 */
int checkInput(const char *command, const char *arg1, const char *arg2) {
  if (strlen(command) > 2 || command[0] != '@') {
    handleError(ERROR_BAD_OP);
    return 1;
  }

  // Match operation command to known operation
  switch (command[1]) {
  case 'n':
    // Error if arg1 is empty or arg2 is not empty
    if (strlen(arg1) < 1 || strlen(arg2) > 0) {
      handleError(ERROR_BAD_ARG);
      return 1;
    }
    return 0;

  case 'e':
    // Error if arg1 is empty or arg2 is empty
    if (strlen(arg1) < 1 || strlen(arg2) < 1) {
      handleError(ERROR_BAD_ARG);
      return 1;
    }
    return 0;

  case 'q':
    // Error if arg1 is empty or arg2 is empty
    if (strlen(arg1) < 1 || strlen(arg2) < 1) {
      handleError(ERROR_BAD_ARG);
      return 1;
    }
    return 0;

  default:
    handleError(ERROR_BAD_OP);
    return 1;
  }
}

/**
 * handleError
 * Purpose: Outputs an error message based on the error code provided.
 * Arguments:
 *   - errorCode: an enum representing the type of error encountered.
 * Returns: Nothing.
 * Assumptions: errorType is a valid error identifier; handles known errors.
 */
void handleError(ErrorType errorType) {
  switch (errorType) {
  case ERROR_BAD_OP:
    fprintf(stderr, "Error: command did not match known operation\n");
    break;

  case ERROR_BAD_ARG:
    fprintf(stderr, "Error: operation @q had bad argument\n");
    break;

  case ERROR_CLI_TOO_MANY_ARGS:
    fprintf(stderr, "Error: too many arguments provided in cli\n");
    break;

  case ERROR_DUPLICATE_NODE:
    fprintf(stderr, "Error: attempted to add an pre-existing node\n");
    break;

  case ERROR_EDGE_MISSING_NODE:
    fprintf(stderr,
            "Error: attempted to make an edge with a non-existing node\n");
    break;

  case ERROR_BAD_INPUT_FILE:
    fprintf(stderr, "Error: input file cannot be found or cannot be read\n");
    break;
  case ERROR_MALLOC:
    fprintf(stderr, "ERROR: Ran out of memory\n");
    break;
  }
}

/**
 * initializeGraph
 * Purpose: Initializes and returns a new, empty graph structure.
 * Arguments: None.
 * Returns: A pointer to a newly allocated graph structure.
 * Assumptions: Caller is responsible for freeing the allocated graph.
 * Notes: Initializes all necessary fields and sets up the graph for node/edge
 * additions.
 */
Node *initializeGraph() {
  Node *head_ptr = malloc(sizeof(Node));

  if (head_ptr == NULL) {
    fprintf(stderr, "Error: Memory ran out\n");
    exit(1);
  }

  head_ptr->name = NULL;
  head_ptr->edgeList = NULL;
  head_ptr->next = NULL;
  head_ptr->visited = 0;
  return head_ptr;
}

/**
 * freeGraph
 * Purpose: Frees all dynamically allocated memory within the graph structure.
 * Arguments:
 *   - graph: a pointer to the graph to free.
 * Returns: Nothing.
 * Assumptions: graph is a valid pointer to an allocated node.
 */
void freeGraph(Node *graph) {
  Node *current = graph;
  while (current != NULL) {
    free(current->name);
    freeEdges(current->edgeList);
    Node *next = current->next;
    free(current);
    current = next;
  }
}

/**
 * freeEdges
 * Purpose: Helps freeGraph free the edges
 * Arguments:
 *   - edgeList: a pointer to the edge to free.
 * Returns: Nothing.
 * Assumptions: edgeList is a valid pointer to an allocated edge.
 */
void freeEdges(Edge *edgeList) {
  if (edgeList == NULL)
    return;

  freeEdges(edgeList->next);
  free(edgeList);
}

/*
 * addNode
 * Purpose: Adds a new vertex to the graph.
 * Arguments:
 *   - graph: a pointer to the graph structure.
 *   - nodeName: a string containing the name of the vertex to add.
 * Returns: 0 if the node is added successfully, 1 if the node already exists.
 * Assumptions: nodeName is unique within the graph.
 * Notes: Protects against buffer overflow by limiting nodeName length.
 */
int addNode(Node *graph, const char *nodeName) {
  if (graph->name == NULL) {
    graph->name = malloc((strlen(nodeName) + 1) * sizeof(char));
    if (graph->name == NULL) {
      handleError(ERROR_MALLOC);
      exit(1);
    }
    strcpy(graph->name, nodeName);
    graph->edgeList = NULL;
    graph->next = NULL;
    graph->visited = 0;
    return 0;
  }

  // Check if graph contains node with name nodeName
  Node *ptr = graph;
  while (ptr != NULL) {

    if (strcmp(ptr->name, nodeName) == 0) {
      handleError(ERROR_DUPLICATE_NODE);
      return 1;
    }

    ptr = ptr->next;
  }

  // Allocate memory
  Node *newNode = malloc(sizeof(Node));
  if (newNode == NULL) {
    handleError(ERROR_MALLOC);
    exit(1);
  }
  newNode->name = malloc((strlen(nodeName) + 1) * sizeof(char));
  strcpy(newNode->name, nodeName);
  newNode->edgeList = NULL;
  newNode->next = NULL;
  newNode->visited = 0;

  // Go to end of list
  while (graph->next != NULL)
    graph = graph->next;

  // Add newNode
  graph->next = newNode;

  return 0;
}

/*
 * addEdge
 * Purpose: Adds a directed edge from one vertex to another in the graph.
 * Arguments:
 *   - graph: a pointer to the graph structure.
 *   - vName1, vName2: strings containing the names of the start and end
 * vertices. Returns: 0 if the edge is added successfully, 1 if an error occurs
 * (e.g., vertices not found). Assumptions: vName1 and vName2 are existing
 * vertices in the graph. Notes: Does nothing if the edge already exists.
 */
int addEdge(Node *graph, const char *vName1, const char *vName2) {
  // Check nodes
  Node *ptr = graph;
  int fromFound = 0;
  int toFound = 0;
  Node *from = NULL;
  Node *to = NULL;

  while (ptr != NULL) {
    if (strcmp(ptr->name, vName1) == 0) {
      fromFound = 1;
      from = ptr;
    }
    if (strcmp(ptr->name, vName2) == 0) {
      toFound = 1;
      to = ptr;
    }
    if (fromFound && toFound)
      break;

    ptr = ptr->next;
  }

  if (!fromFound || !toFound) {
    handleError(ERROR_EDGE_MISSING_NODE);
    return 1;
  }

  // Look for duplicate edge
  Edge *edge = from->edgeList;
  Edge *end = NULL;

  while (edge != NULL) {
    if (strcmp(edge->destination->name, vName2) == 0) {
      // Edge already exists
      return 0;
    }
    end = edge;
    edge = edge->next;
  }

  // Allocate and add new edge
  Edge *newEdge = malloc(sizeof(Edge));
  if (!newEdge) {
    perror("Failed to allocate memory for new edge");
    exit(1);
  }
  newEdge->destination = to;
  newEdge->next = NULL;

  if (from->edgeList == NULL) {
    // First edge for this node
    from->edgeList = newEdge;
  } else {
    // Append to the end of the list
    end->next = newEdge;
  }

  return 0;
}

/**
 * searchPath
 * Purpose: Determines if a directed path exists between two vertices using DFS.
 * Arguments:
 *   - graph: a pointer to the graph structure.
 *   - startNode, endNode: strings representing the names of the start and end
 * vertices. Returns: 1 if a path exists, 0 otherwise. Assumptions: startNode
 * and endNode are existing vertices. Notes: Calls depthFirstSearch to perform
 * the DFS traversal.
 */
int searchPath(Node *graph, const char *startNode, const char *endNode) {
  Node *ptr = graph;
  Node *start = NULL;
  Node *end = NULL;

  while (ptr != NULL) {
    if (strcmp(ptr->name, startNode) == 0) {
      start = ptr;
    }
    if (strcmp(ptr->name, endNode) == 0) {
      end = ptr;
    }

    // Exit early if both found
    if (start != NULL && end != NULL)
      break;

    ptr = ptr->next;
  }

  int result = depthFirstSearch(start, end);

  Node *new_ptr = graph;
  while (new_ptr != NULL) {
    new_ptr->visited = 0;
    new_ptr = new_ptr->next;
  }

  return result;
}

/**
 * depthFirstSearch
 * Purpose: Recursively searches for a path from the current node to the target
 * node.
 * Arguments:
 *   - graph: a pointer to the graph structure.
 *   - currentNode: a pointer to the current node being explored.
 *   - targetNode: a pointer to the target node.
 *   - visited: an array marking visited nodes to avoid cycles.
 * Returns: 1 if a path is found, 0 if no path exists.
 * Assumptions: visited array is pre-initialized; graph is valid.
 */
int depthFirstSearch(Node *currentNode, Node *targetNode) {
  // Implementation here
  if (currentNode == targetNode) {
    return 1;
  }

  if (currentNode->visited == 1) {
    return 0;
  }

  currentNode->visited = 1;

  Edge *ptr = currentNode->edgeList;
  while (ptr != NULL) {
    if (depthFirstSearch(ptr->destination, targetNode))
      return 1;
    ptr = ptr->next;
  }

  return 0;
}
