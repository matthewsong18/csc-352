#ifndef QUEUE_H
#define QUEUE_H

#include "graph.h"

typedef struct QueueNode {
    ActorNode *actor;
    struct QueueNode *next;
} QueueNode;

struct Queue {
    QueueNode *front;
    QueueNode *rear;
};

typedef struct Queue Queue;

Queue *init_queue();
void enqueue(Queue *queue, ActorNode *actor);
ActorNode *dequeue(Queue *queue);
void free_queue(Queue *queue);

#endif
