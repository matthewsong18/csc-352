# Assignment 7

## Prob 1: Reach

### Inputs

- **Command-line Argument**: Accepts an optional file name. If specified, the
  program reads input from the file; otherwise, it reads from standard input.

- **Input Format**: Each line of input is in the format `op args`, where `op` is
  one of:

  - `@n vName`: Declares `vName` as a vertex in the graph. `vName` is an
    alphanumeric string, up to 64 characters.

  - `@e vName1 vName2`: Declares a directed edge from `vName1` to `vName2`. Both
    vertices must already exist.

  - `@q vName1 vName2`: Checks if there is a directed path from `vName1` to
    `vName2`. Outputs `1` if a path exists, `0` otherwise.

### Outputs

- For each `@q` command, prints `1` if a path exists, `0` otherwise.

- The program exits with status:

  - `0` for normal execution.

  - `1` if any errors were encountered during processing.

### Error Conditions

- **Non-fatal Errors**:

  - More than one command-line argument (ignore extras).

  - Invalid input lines (skip invalid lines, print an error message, continue).

  - Duplicate vertex declarations.

  - Edge declarations with non-existent nodes.

- **Fatal Errors**:

  - Specified file does not exist or is unreadable.

### Special Requirements

- **Data Structure**: Use an adjacency list represented with two structs—one for
  nodes and another for directed edges.

  - Nodes are stored in a linked list, with each node having a linked list of
    edges representing outgoing connections.

- **Algorithm**: Implement depth-first search to determine reachability.

- **Memory Management**: Use `malloc`/`calloc` for dynamic memory allocation,
  check for `NULL` on allocation failures, and free all memory for extra credit.

- **Execution Requirements**:

  - Compile with `-Wall` to prevent warnings.

  - Code must pass valgrind checks (`ERROR SUMMARY: 0 errors`).
