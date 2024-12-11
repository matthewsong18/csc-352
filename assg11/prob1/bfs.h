#ifndef BFS_H
#define BFS_H

#include "graph.h"

int calculate_bacon_score(Graph *graph, const char *start_actor);
char *trace_path(Graph *graph, const char *start_actor);

#endif
