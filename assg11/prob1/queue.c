#include "queue.h"
#include "graph.h"
#include <stdlib.h>

Queue *init_queue() {
  Queue *q = malloc(sizeof(Queue));
  if (!q)
    return NULL;
  q->front = NULL;
  q->rear = NULL;
  return q;
}

void enqueue(Queue *queue, ActorNode *actor) {
  if (!queue || !actor)
    return;
  QueueNode *node = malloc(sizeof(QueueNode));
  if (!node)
    return; // If allocation fails, ideally handle gracefully
  node->actor = actor;
  node->next = NULL;
  if (!queue->rear) {
    // Empty queue
    queue->front = node;
    queue->rear = node;
  } else {
    queue->rear->next = node;
    queue->rear = node;
  }
}

ActorNode *dequeue(Queue *queue) {
  if (!queue || !queue->front)
    return NULL;
  QueueNode *temp = queue->front;
  ActorNode *actor = temp->actor;
  queue->front = queue->front->next;
  if (!queue->front) {
    // The queue is now empty
    queue->rear = NULL;
  }
  free(temp);
  return actor;
}

void free_queue(Queue *queue) {
  if (!queue)
    return;
  // Dequeue all nodes
  while (queue->front) {
    QueueNode *temp = queue->front;
    queue->front = queue->front->next;
    free(temp);
  }
  free(queue);
}
