#include "priority_queue.h"
#include <stdio.h>


void pq_init(PriorityQueue *q)
{
    q->size=0;
}

uint8_t pq_empty(PriorityQueue *q)
{
    return q->size==0;
}

uint8_t pq_full(PriorityQueue *q)
{
    return q->size==QUEUE_SIZE;
}

uint8_t pq_push(PriorityQueue *q, can_msg_t *msg)
{
    if(q->size < QUEUE_SIZE) q->size++;
    int i = q->size;
    while (i>0 && q->msgs[i].sid > msg->sid)
    {
        q->msgs[i]=q->msgs[i-1];
        i--;
    }
    q->msgs[i] = *msg;
    return 0;
}

uint8_t pq_pop(PriorityQueue *q, can_msg_t *msg_out)
{
    if(pq_empty(q))
    {
        return 1;
    }
    
    *msg_out = q->msgs[0];
    
    //shift rest of messages up in queue
    for(int i =1; i<q->size;i++)
    {
        q->msgs[i-1]=q->msgs[i];
    }
    
    q->size--;
    return 0;
}

uint8_t pq_peek(PriorityQueue *q, can_msg_t *msg_out)
{
    if(pq_empty(q))
    {
        return 1;
    } 
    *msg_out=q->msgs[0];
    return 0;
}
