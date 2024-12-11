#ifndef QUEUE_H
#define QUEUE_H

#include "graph.h"

typedef struct Queue Queue;

Queue *init_queue();
void enqueue(Queue *queue, ActorNode *actor);
ActorNode *dequeue(Queue *queue);
void free_queue(Queue *queue);

#endif
