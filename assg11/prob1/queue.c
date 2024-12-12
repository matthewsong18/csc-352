/*
 * File: queue.c
 * Author: Matthew W. Song
 * Purpose: Implements a queue data structure for managing ActorNode objects
 *          during breadth-first search (BFS) traversal.
 */

#include "queue.h"
#include "graph.h"
#include <stdlib.h>

/*
 * init_queue()
 * Initializes an empty queue.
 *
 * Returns:
 *   Pointer to the newly created Queue structure, or NULL on failure.
 */
Queue *init_queue() {
  Queue *q = malloc(sizeof(Queue));
  if (!q)
    return NULL;
  q->front = NULL;
  q->rear = NULL;
  return q;
}

/*
 * enqueue(queue, actor)
 * Adds an ActorNode to the rear of the queue.
 *
 * Arguments:
 *   queue - Pointer to the Queue structure.
 *   actor - Pointer to the ActorNode to enqueue.
 */
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

/*
 * dequeue(queue)
 * Removes and returns the ActorNode at the front of the queue.
 *
 * Arguments:
 *   queue - Pointer to the Queue structure.
 *
 * Returns:
 *   Pointer to the ActorNode dequeued, or NULL if the queue is empty.
 */
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

/*
 * free_queue(queue)
 * Frees all memory associated with a Queue structure and its nodes.
 *
 * Arguments:
 *   queue - Pointer to the Queue structure to free.
 */
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
