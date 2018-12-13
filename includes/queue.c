#include "queue.h"

Queue *ConstructQueue(int limit)
{
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL)
    {
        return NULL;
    }
    if (limit <= 0)
    {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void DestructQueue(Queue *queue)
{
    queue_node *pN;
    while (!isEmpty(queue))
    {
        pN = Dequeue(queue);
        free(pN);
    }
    free(queue);
}

int Enqueue(Queue *pQueue, queue_node *item)
{
    /* Bad parameter */
    if ((pQueue == NULL) || (item == NULL))
    {
        return FALSE;
    }
    // if(pQueue->limit != 0)
    if (pQueue->size >= pQueue->limit)
    {
        return FALSE;
    }
    /*the queue is empty*/
    item->prev = NULL;
    if (pQueue->size == 0)
    {
        pQueue->head = item;
        pQueue->tail = item;
    }
    else
    {
        /*adding item to the end of the queue*/
        pQueue->tail->prev = item;
        pQueue->tail = item;
    }
    pQueue->size++;
    return TRUE;
}

queue_node *Dequeue(Queue *pQueue)
{
    /*the queue is empty or bad param*/
    queue_node *item;
    if (isEmpty(pQueue))
        return NULL;
    item = pQueue->head;
    pQueue->head = (pQueue->head)->prev;
    pQueue->size--;
    return item;
}

int isEmpty(Queue *pQueue)
{
    if (pQueue == NULL)
    {
        return FALSE;
    }
    if (pQueue->size == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
