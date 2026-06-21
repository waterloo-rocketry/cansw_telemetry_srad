#include "priority_queue.h"
#include <stdio.h>


void pq_init(CAN_PriorityQueue *q)
{
    q->size=0;
}

uint8_t pq_empty(CAN_PriorityQueue *q)
{
    return q->size==0;
}

uint8_t pq_full(CAN_PriorityQueue *q)
{
    return q->size==QUEUE_SIZE;
}

uint8_t pq_push(CAN_PriorityQueue *q, can_msg_t *msg)
{
    if(pq_full(q))
    {
        return 1;
    }
    int i = q->size;
    while (i>0 && q->msgs[i-1].sid > msg->sid)
    {
        q->msgs[i]=q->msgs[i-1];
        i--;
    }
    q->msgs[i]=*msg;
    q->size++;
    return 0;
}

uint8_t pq_pop(CAN_PriorityQueue *q, can_msg_t *msg_out)
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

uint8_t pq_peek(CAN_PriorityQueue *q, can_msg_t *msg_out)
{
    if(pq_empty(q))
    {
        return 1;
    } 
    *msg_out=q->msgs[0];
    return 0;
}