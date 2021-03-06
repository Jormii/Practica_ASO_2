#pragma once
#include <stdlib.h>
#include <stdio.h>

/**
 * Fuente: https://gist.github.com/ArnonEilat/4471278
 */

#define TRUE 1
#define FALSE 0

typedef struct queue_node_t queue_node;

typedef struct queue_node_t
{
    unsigned int tamano;
    unsigned int matricula;
    queue_node *prev;
} queue_node;

/* the HEAD of the Queue, hold the amount of node's that are in the queue*/
typedef struct Queue
{
    queue_node *head;
    queue_node *tail;
    int size;
    int limit;
} Queue;

Queue *ConstructQueue(int limit);
void DestructQueue(Queue *queue);
int Enqueue(Queue *pQueue, queue_node *item);
queue_node *Dequeue(Queue *pQueue);
int isEmpty(Queue *pQueue);